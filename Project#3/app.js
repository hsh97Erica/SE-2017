var express = require('express');
var path = require('path');
var favicon = require('serve-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');

var index = require('./routes/index');
var users = require('./routes/users');
var session = require('express-session');
var app = express();
var cheerio = require('cheerio');
var requestlib = require('request');
var rssparser = require('rss-parser');
var mysql      = require('mysql');
var connection = mysql.createPool({
  connectionLimit : 10,
  host     : 'localhost',
  user     : 'guest',
  password : '',
  database: 'logindb'
});
var checkinjectionquery = function(qry){
  if(qry.includes("'")||qry.includes("'")||qry.includes("/**/")||qry.includes("#")||qry.includes(",")||(qry.includes("/*")&&qry.includes("*/"))){
    return true;
  }else{
    return false;
  }
};
// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'jade');
app.use(session({
  resave:false,
  secret: '#$%^&!@memoapp&*()',
  saveUninitialized:true,
  //cookie: { secure: false }
}));
// uncomment after placing your favicon in /public
//app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', index);
app.use('/edv', express.static(__dirname + '/'));
app.use('/view1', express.static(__dirname + '/'));
app.use('/view2', express.static(__dirname + '/'));
app.use('/view3', express.static(__dirname + '/'));
app.use('/daemonview', express.static(__dirname + '/'));
app.use('/users', users);
app.post("/accountmgr/:id/modify",function(req,res){
  const mid = req.params.id;
  console.log(JSON.stringify(req.body));
  const currpw = req.body.currpw;
  const newpw = req.body.newpw;
  const nn = req.body.nn;
  if(mid===undefined||currpw===undefined||(mid!=undefined&&(mid.includes("'")||mid.includes('"')))||(currpw!=undefined&&(currpw.includes("'")||currpw.includes('"')))||(newpw!=undefined&&(newpw.includes("'")||newpw.includes('"')))||(nn!=undefined&&(nn.includes("'")||nn.includes('"')))){
    console.log("some variable is invalid\n"+mid+"\n"+currpw+"\n"+newpw+"\n"+nn);
    res.json({apply:false});
  }else{
    connection.getConnection(function(err,conn){
      if(err){res.json({apply:false});throw err;}
      conn.query("select count(*) as cnt from logindb.accountlist where id='"+mid+"' and pw='"+currpw+"';",function(err, rows, fields){
        if(err){res.json({apply:false});throw err;}
        if(rows[0].cnt<1){
          console.log("there is no account data");
          res.json({apply:false});
        }else{
          conn.changeUser({user : 'logineduser',password:'logineduser123',database:'logindb'}, function(err) {
            if(err){res.json({apply:false});throw err;}
            if((newpw!=undefined&&(newpw.trim()!=""&&newpw.trim().length>0))&&(nn!=undefined&&(nn.trim()!=""&&nn.trim().length>0))){
              conn.query("update logindb.accountlist set pw='"+newpw+"' where id='"+mid+"';",function(err, rows, fields){
                if(err){res.json({apply:false});throw err;}
                conn.commit(function(err){if(err){res.json({apply:false});throw err;}
                sess = req.session;
              sess.upw = newpw;
              sess.save();
                
              
              conn.query("update logindb.user_info set nickname='"+nn+"' where id='"+mid+"';",function(err, rows, fields){
                if(err){res.json({apply:false});throw err;}
                conn.commit(function(err){if(err){res.json({apply:false});throw err;}
                sess = req.session;
                sess.nickname =  nn;      
                sess.save();
              res.json({apply:true});
            });
              });
             });
              });
            }
            else if(newpw!=undefined&&(newpw.trim()!=""&&newpw.trim().length>0)){
              conn.query("update logindb.accountlist set pw='"+newpw+"' where id='"+mid+"';",function(err, rows, fields){
                if(err){res.json({apply:false});throw err;}
                conn.commit(function(err){
                  if(err){res.json({apply:false});throw err;}
                  sess = req.session;
                  sess.upw = newpw;
                  sess.save();
                  res.json({apply:true});
                  console.log("newpw update commit ok");
                  });
                
              });
            }
            else if(nn!=undefined&&(nn.trim()!=""&&nn.trim().length>0)){
              conn.query("update logindb.user_info set nickname='"+nn+"' where id='"+mid+"';",function(err, rows, fields){
                if(err){res.json({apply:false});throw err;}
                conn.commit(function(err){
                  if(err){res.json({apply:false});throw err;}
                  sess = req.session;
                sess.nickname =  nn;      
                sess.save();
                  res.json({apply:true});
                  console.log("nickname update commit ok :)");
                });
                
              });
            }
          });
        }
      });
    });
  }
});
app.post("/accountmgr/:id/addnewuser",function(req,res){
  const mid = request.params.id;
  const currpw = request.body.currpw;
  const newpw = request.body.newpw;
  const nn = request.body.nn;

});
app.post("/accountmgr/:id/duplicationcheck",function(req,res){
  const mid = request.params.id;
  const currpw = request.body.currpw;
  const newpw = request.body.newpw;
  const nn = request.body.nn;

});
app.get("/addons/naverrank.json",function(req,res){
  var url = "https://m.naver.com/";
  requestlib(url, function(error, response, html){  
    if (error) {res.json({value:"error"});throw error};
    var $ = cheerio.load(html);
   // console.log("cheerio loaded");
    const rst = [];
    $('script').each(function(){
        var hc = $(this).html();
        if(hc.includes("oRTK :")){
            var newdtstr = hc.substring(hc.indexOf("oRTK :")+"oRTK :".length,hc.indexOf("oHTP :")-2).trim();
            var jsonobj = JSON.parse(newdtstr);
            var ranklst = jsonobj.d;
            for(i=0;i<ranklst.length;i++){
                rst.push(ranklst[i].k.trim());
            }
           // console.log(jsonobj.d);
        }
      });
      console.log(rst);
      res.json(rst);//{orderedlist:rst});
});
});
app.get("/addons/universitynotice.json",function(req,res){
   var forceerr = false;
    if(forceerr){
      var rst = {};
  rst['iserror'] = false;
      rst['feed_title'] = "parsed.feed.title";
       // rst['iserror'] = false;
       lst = [{"title":"[민간산업체] 삼성전자 미래기술육성센터 지정테마 과제공모","link":"http://portal.hanyang.ac.kr:80/openPop.do?url=openPage.do%3FpgmId%3DP308981%26tk%3Da0bfa98b7a872b2f54d2b55ec4ae27624d076e48c4f690368e92f6c4d320d859&gongjiSeq=9329&header=hidden","pdate":"Thu, 13 Apr 2017 15:00:00 GMT","author":"서울 산학협력단 연구지원팀/최소영"},{"title":"서울캠퍼스 2017학년도 여름계절학기 학점교류 신청 안내","link":"http://portal.hanyang.ac.kr:80/openPop.do?url=openPage.do%3FpgmId%3DP308981%26tk%3Da0bfa98b7a872b2f54d2b55ec4ae27624d076e48c4f690368e92f6c4d320d859&gongjiSeq=9327&header=hidden","pdate":"Thu, 13 Apr 2017 15:00:00 GMT","author":"서울 교무처 학사팀/황수미"}];
       //lst.push({title:"entry.title",link:"mlink",pdate:"entry.pubDate",author:"entry.author"});
       
        rst['link_host'] = "parsed.feed.link";
        rst['data'] = lst;
      res.json(rst);
}
      else
  rssparser.parseURL('https://portal.hanyang.ac.kr/GjshAct/viewRSS.do?gubun=rss',{maxRedirects: 3}, function(err, parsed) {
    
    const rst = {};
    
    if(err){
      rst['iserror'] = true;
      rst['feed_title'] = "parsed.feed.title";
       // rst['iserror'] = false;
       lst = [{"title":"[민간산업체] 삼성전자 미래기술육성센터 지정테마 과제공모","link":"http://portal.hanyang.ac.kr:80/openPop.do?url=openPage.do%3FpgmId%3DP308981%26tk%3Da0bfa98b7a872b2f54d2b55ec4ae27624d076e48c4f690368e92f6c4d320d859&gongjiSeq=9329&header=hidden","pdate":"Thu, 13 Apr 2017 15:00:00 GMT","author":"서울 산학협력단 연구지원팀/최소영"},{"title":"서울캠퍼스 2017학년도 여름계절학기 학점교류 신청 안내","link":"http://portal.hanyang.ac.kr:80/openPop.do?url=openPage.do%3FpgmId%3DP308981%26tk%3Da0bfa98b7a872b2f54d2b55ec4ae27624d076e48c4f690368e92f6c4d320d859&gongjiSeq=9327&header=hidden","pdate":"Thu, 13 Apr 2017 15:00:00 GMT","author":"서울 교무처 학사팀/황수미"}];
       //lst.push({title:"entry.title",link:"mlink",pdate:"entry.pubDate",author:"entry.author"});
       rst['data'] = lst;
        rst['link_host'] = "parsed.feed.link";
      res.json(rst);
     // throw err;
    }else{
        rst['feed_title'] = parsed.feed.title;
        rst['iserror'] = false;
        rst['link_host'] = parsed.feed.link;
        const lst = [];
        console.log(parsed.feed.title);
        parsed.feed.entries.forEach(function(entry) {
          var mlink = rst.link_host+(entry.link);
          if(lst.length<=30){
          lst.push({title:entry.title,link:mlink,pdate:entry.pubDate,author:entry.author});
        }
        else{
          return;
        }
        // console.log(entry.title + ':' + " "+entry.pubDate);
        });
        rst['data']=lst;
        res.json(rst);
    }
});
});
app.delete("/dologin.json",function(req,res){
  connection.getConnection(function(err,conn){
        connection.getConnection(function(err,conn){
          if(err){res.json({d:false});throw err;}
          conn.changeUser({user : 'logineduser',password:'logineduser123',database:'logindb'}, function(err) {
            if(err){res.json({d:false});throw err;}
            conn.query("delete from logindb.loginhash where hashv=md5('"+req.sessionID+"');",function(err, rows, fields){
              if(err){res.json({d:false});throw err;}
              conn.commit(function(err){if(err){res.json({d:false});throw err;}});
              req.session.destory;
              res.clearCookie('sid');
              req.session=null;
              res.json({d:true});
            });
          });
        });
    });
});
var lg = express.Router().get("/",function(request,response){
connection.query("use logindb;",function(err,rows,fields){});
  //try{connection.connect(); }catch(err2){}
console.log("session id: "+request.session.uid);
connection.query('SELECT * from availableloginlist;', function(err, rows, fields) {//1차적인 sql injection 방어
  if (err) {response.json({logined:false});throw err;}
  //if(err)return;
  const rst = {};
     tmpid = request.session.uid===undefined?  request.query.id:request.session.uid;
     tmppw = request.session.uid===undefined? request.query.pw:request.session.upw;
loop = false;

  for(i=0;i<rows.length;i++){
    const chkid = tmpid;
    const pw = tmppw;
    if(chkid!=null&&pw!=null&&pw.trim()!=''&&chkid.trim()!=''&&!(pw.includes("'")|| pw.includes('"'))&&!(chkid.includes("'")|| chkid.includes('"'))&&rows[i].pw===pw&&rows[i].id===chkid){
      loop=true;
      console.log("condition check ok");
      rst["logined"]=true;
      const timestamp =  new Date().getTime();
      const expirets = timestamp+60*60*12*1000;
      var chkqur ="SELECT count(*) as cnt from loginhash where id='"+chkid+"' and hashv=md5('"+request.sessionID+"') ;";
      console.log("cnt qry: "+chkqur);
      connection.query(chkqur,function(err2,rows2,fields2){
        if (err2) {
          response.json({logined:false});
          console.log("error at query");
          throw err2;
        }
        console.log(rows2[0].cnt);
        if(rows2[0].cnt ==0){
          console.log("before getconnection");
          connection.getConnection(function(err,conn){
                conn.beginTransaction(function(err){
                
                    if(err){
                      response.json({logined:false});
                      console.log("error at beginTrans");
                      throw err;
                    }
                    console.log("pass begin transaction.");
                  var mQuery = "insert into loginhash(id,hashv) values('"+chkid+"', md5('"+(request.sessionID)+"') "+");";
                    sess = request.session;
                    sess.uid = chkid;
                    sess.upw = pw;
                //sess.nickname = 
              // sess.save();
                    console.log(mQuery);
                    console.log("uid: "+sess.uid);
                    conn.query(mQuery,function(err3, rows3, fields3) {
                    conn.commit(function(err){

                        if(err){
                          conn.rollback(function(){
                            console.log("error at rollback");
                            response.json({logined:false});
                            throw err;
                          });
                        }
              // request.session.uid = chkid;
                    });
                  });
                  var mQuery2 = "select nickname from user_info where id='"+chkid+"';";
                  console.log(mQuery2);
                  conn.query(mQuery2,function(err3, rows3, fields3) {

                      if(err){
                            response.json({logined:false});
                          throw err;
                      }
                ///sess = request.session;
                      sess.nickname =  rows3[0].nickname;      
                      sess.save();
                      var nn = sess.nickname;
                      response.json({logined:true,id:chkid,nickname:nn});
              // console.log("nicknameval: "+sess.nickname);
          
              // request.session.uid = chkid;
            
              });

              });
          //connection.query("commit;",function(err3, rows3, fields3) {});
        });
        /*sess = request.session;
        console.log("var nn: "+nn);
        response.json({logined:true,id:chkid,nickname:nn});*/
      }
      else{
        console.log("login else proc");
        if(request.session.uid===undefined){
          console.log("login session undefined ")
           connection.getConnection(function(err,conn){
          if(err){response.json({logined:false});throw err;}

          const qry = "delete from loginhash where hashv=md5('"+request.sessionID+"');";
         
            conn.query(qry,function(err,rows,fields){
              if(err){response.json({logined:false});throw err;}
               conn.commit(function(err){response.json({logined:false});if(err)throw err;});
            });
            response.json({logined:false});
        });
          
        }else{
          const secid = request.session.uid;
          const nn = request.session.nickname;
       var qry ="SELECT count(*) as cnt from loginhash where id='"+secid+"' and hashv=md5('"+request.sessionID+"') ;";
        
        connection.query( qry,function(err,rowss2,fields){
          if(err){response.json({logined:false});throw err;}
          if(rowss2[0].cnt==1){
            response.json({logined:true,id:secid,nickname:nn});
          }
          else{
            response.json({logined:false});
          }
        });

        }
      }
      });
      
      break;
    }
  }
  if(loop==false){
    console.log("loop false work");
    response.json(rst);
  }
  //response.json(rst);
});
//connection.end();
});
app.get("/memotransac/:id/mtdlt.json",function(request,response){
  const method = request.method;
  const id = request.params.id;
 //console.log("!"+JSON.stringify(JSON.parse(request.body))+"!");
  const pid = request.query.postid;
  console.log("header method: "+method);
   rst = {};
    connection.getConnection(function(err,conn){
          
          if(err)throw err;
          console.log("open connection ok");
      conn.changeUser({user : 'logineduser',password:'logineduser123',database:'memodb'}, function(err) {
        if(err)throw err;
        conn.query("select count(*) as cnt from memoview where id='"+id+"' and boardnumber="+pid+";",function(err,rows,fields){
          if(err){
            try{response.json({state:false});
    }catch(err2){}
            throw err;
          }
          
          if(rows[0].cnt>0){
            var qryy = "delete from memolist where boardnumber="+pid+";";
            console.log(qryy);
            conn.query(qryy,function(err,rows,fields){
              if(err){
                   try{response.json({state:false});
                    }catch(err2){}
                throw err;
              }
              conn.commit(function(err){try{response.json({state:false});
                    }catch(err2){}});
              response.json({state:true});
            });
            
          }
          else{
            response.json({state:false});
          }
          //response.json({state:true});
        });
      });
    });
    
});



app.post("/memotransac/:id/mtut.json",function(request,response){
  const method = request.method;
  const id = request.params.id;
 console.log("!"+JSON.stringify(request.body)+"!");
 //console.log("!"+JSON.stringify(JSON.parse(request.body))+"!");
  const bpw = request.body.boardpw;
  const pid = request.body.postid;
  const memodata = request.body.mdt;
  const title = request.body.title;
  console.log("header method: "+method);
   rst = {};
    connection.getConnection(function(err,conn){
          
          if(err)throw err;
          console.log("open connection ok");
      conn.changeUser({user : 'logineduser',password:'logineduser123',database:'memodb'}, function(err) {
        if(err)throw err;
        conn.query("select count(*) as cnt from memoview where id='"+id+"' and boardnumber="+pid+";",function(err,rows,fields){
          if(err){
            try{response.json({state:false});
    }catch(err2){}
            throw err;
          }
          if(rows[0].cnt>0){
            conn.query("update memocontent set memodata='"+memodata+"' , title='"+title+"', modify_date=NOW() where boardnumber="+pid+";",function(err,rows,fields){
              if(err){
                   try{response.json({state:false});}catch(err2){}
                throw err;
              }
              response.json({state:true});
            });
            
          }else{
            conn.query("insert into memolist(id) values('"+id+"');",function(err,rows,fields){
              console.log("insert pid ok");
              conn.commit(function(err){if(err)throw err;});
              console.log("commit of insert pid ok");
            conn.query("select boardnumber as bn from memolist where id='"+id+"' order by boardnumber desc limit 1;",function(err,rows,fields){
              if(err){
                try{response.json({state:false});}catch(err2){}
                throw err;
              }
               console.log("select boardnumber ok");
            conn.query("insert into memocontent(boardnumber,title,memodata) values("+rows[0].bn+", '"+title+"' , '"+memodata+"');",function(err,rows2,fields){
              if(err){
                try{response.json({state:false});}catch(err2){}
                throw err;
              }
              console.log("insert memocontent ok");
              conn.commit(function(err){if(err){try{response.json({state:false});}catch(err2){}
            throw err;}});

              response.json({state:true,pid:rows[0].bn});
            });
          });
           });
          }
          conn.commit(function(err){if(err)try{response.json({state:false});}catch(err2){}});
          //response.json({state:true});
        });
      });
    });
    
});
app.get("/memotransac/:id/mt.json",function(request,response){
  const method = request.method;
  const id = request.params.id;
  console.log("header method: "+method);
   rst = {};
  switch(method){
    case 'get':
    case 'GET':{
      const bpw = request.query.boardpw;
      const pid = request.query.postid;
      connection.getConnection(function(err,conn){
          
          if(err)throw err;
          console.log("open connection ok");
      conn.changeUser({user : 'logineduser',password:'logineduser123',database:'memodb'}, function(err) {
        if(err)throw err;
        
          if(bpw!=null&&bpw.trim()!=''){
            conn.query("select * from memoview where id='"+id+"' and boardnumber="+pid+" and passwordlocked='"+bpw+"';",function(err,rows,fields){
             if(err)throw err;
             console.log("memo content quary1 ok(l: "+ rows.length);
              rst["post_title"]=rows[0].title;
              rst["content"]=rows[0].memodata.toString();
              rst["modify_date"]= rows[0].modify_date;
              response.json(rst);
              //console.log(rst);
            });
          }else{
            conn.query("select * from memoview where id='"+id+"' and boardnumber="+pid+";",function(err,rows,fields){
              if(err)throw err;
              console.log("memo content quary2 ok(l:"+rows.length);
              rst["post_title"]=rows[0].title;
              rst["content"]=rows[0].memodata.toString();
              rst["modify_date"]= rows[0].modify_date.toString();
              response.json(rst);
             // console.log(rst);
            });
          }
      
        });
     
    });
     break;
    }
    case '':{

    }
    default:break;
  }
  //response.json(rst);
});
//app.post("/memotransac/:id/mt.json",function(request,response){});

app.get("/:id/querydata.json",function(request,response){
  const id = request.params.id;
  //try{connection.connect(); }catch(err2){}
  const pid = request.query.postid;
  logined = null;//"false";
connection.query("use logindb;",function(err,rows,fields){});
const qry2 = "select count(id) as cnt from loginhash where id='"+id+"' and hashv=md5('"+request.sessionID+"');";
console.log(qry2);
connection.query(qry2,function(err,rowss,fields){
  if (err) throw err;
  console.log("cnt qry: "+rowss[0].cnt);
  if(rowss[0].cnt>0){
     logined = "true";
  }
//});
//console.log("raw lg state: "+logined);
logined =  Boolean(logined);
console.log(logined);
if(rowss[0].cnt>0){//logined){
  console.log("logined passed ");
  connection.getConnection(function(err,conn){
  conn.changeUser({user : 'logineduser',password:'logineduser123',database:'memodb'}, function(err) {
 
//connection.query("use memodb;",function(err,rows,fields){
  if (err) throw err;
conn.query("select boardnumber,title,memodata,modify_date as md from memoview where id='"+id+"' and boardnumber="+pid+" ;",function(err,rows,fields){
    if (err) throw err;
    const rst = {list_count:rows.length,data:[]};
    console.log("select pass and len="+rows.length);
    if(rows.length==0){
      rst["resp"] = 'data query is succeed ,but no data';
      response.json(rst)
    }else{
    for(i=0;i<rows.length;i++){
      rows[i]["memodata"]=rows[i]["memodata"].toString();
        rst["data"].push(rows[i]);
      //response.json(rows[i]);
      break;
      //rst.values.push();
    }
    response.json(rst);
    }
});
});
  });
}
else{
  response.json(logined);
}
});
});
///var queryMemoList = 
//express.Router().



app.get("/:id/inquirylist.json",function(request,response){
  const id = request.params.id;
  //try{connection.connect(); }catch(err2){}
  
var logined = "false";
connection.query("use logindb;",function(err,rows,fields){});
const qry2 = "select count(id) as cnt from loginhash where id='"+id+"' and hashv=md5('"+request.sessionID+"');";
console.log(qry2);
connection.query(qry2,function(err,rows,fields){
  if (err) throw err;
  console.log("cnt qry: "+rows[0].cnt);
  if(rows[0].cnt>0){
     logined = "true";
  }
});
logined = Boolean(logined);
console.log(logined);
if(logined){
  console.log("logined passed ");
  connection.getConnection(function(err,conn){
  conn.changeUser({user : 'logineduser',password:'logineduser123',database:'memodb'}, function(err) {
 
//connection.query("use memodb;",function(err,rows,fields){
  if (err) throw err;
conn.query("select boardnumber,title,passwordlocked,modify_date as md from memoview where id='"+id+"' ;",function(err,rows,fields){
    if (err) throw err;
    const rst = {list_count:rows.length,data:[]};
    console.log("select pass and len="+rows.length);
    if(rows.length==0){
      rst["resp"] = 'data query is succeed ,but no data';
      response.json(rst);
    }else{
    for(i=0;i<rows.length;i++){
        
        rst["data"].push(rows[i]);
      //response.json(rows[i]);
      
      //rst.values.push();
    }
    response.json(rst);
    }
});
});
  });
}
else{
  response.json(logined);
}
});
app.use('/dologin.json',lg);
//app.use('/:id/inquirylist.json',queryMemoList);
// catch 404 and forward to error handler
app.use(function(req, res, next) {
  var err = new Error('Not Found');
  err.status = 404;
  next(err);
});

// error handler
app.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});

module.exports = app;
app.listen(8080,function (){

});
