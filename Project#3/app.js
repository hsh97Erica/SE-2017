var express = require('express');
var path = require('path');
var favicon = require('serve-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');

var index = require('./routes/index');
var users = require('./routes/users');
var session = require('express-session');
var noteapp = express();
var cheerio = require('cheerio');
var requestlib = require('request');
var rssparser = require('rss-parser');
var mysql      = require('mysql');
var base64 =  require('js-base64').Base64;
var aes256 = require('nodejs-aes256');
var connection = mysql.createPool({
  connectionLimit : 10,
  host     : 'localhost',
  user     : 'guest',
  password : '',
  database: 'logindb'
});
var checkinjectionquery = function(qry){
  if(qry.includes('"')||qry.includes("'")||qry.includes(";")||qry.includes("/**/")||qry.includes("#")||qry.includes(",")||(qry.includes("/*")&&qry.includes("*/"))){//원시적 injection 방어
    console.log("[injection query found] base64ed qry: '"+base64.encode(qry)+"'");
    return true;
  }else{
    return false;
  }
};
// view engine setup
noteapp.set('views', path.join(__dirname, 'views'));
noteapp.set('view engine', 'jade');
noteapp.use(session({
  resave:false,
  secret: '#$%^&!@memoapp&*()',
  saveUninitialized:true,
  //cookie: { secure: false }
}));
var chgusrinfo = {user : 'logineduser',password:'logineduser123',database:'memodb'};
// uncomment after placing your favicon in /public
//noteapp.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
noteapp.use(logger('dev'));
noteapp.use(bodyParser.json());
noteapp.use(bodyParser.urlencoded({ extended: false }));
noteapp.use(cookieParser());
noteapp.use(express.static(path.join(__dirname, 'public')));

noteapp.use('/', index);
noteapp.use('/edv', express.static(__dirname + '/'));
noteapp.use('/view1', express.static(__dirname + '/'));
noteapp.use('/view2', express.static(__dirname + '/'));
noteapp.use('/view3', express.static(__dirname + '/'));
noteapp.use('/daemonview', express.static(__dirname + '/'));
noteapp.use('/users', users);
noteapp.post("/accountmgr/:id/modify",function(req,res){
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
noteapp.post("/accountmgr/:id/addnewuser",function(req,res){
  const mid = req.params.id;
  const newpw = base64.decode( req.body.newpw );
  const nn = base64.decode(req.body.nickn);
  console.log("param pass");
  console.log("==="+mid+"\n"+newpw+"\n"+nn+"\n===");
  if(checkinjectionquery(mid)||checkinjectionquery(newpw)||checkinjectionquery(nn)){
    res.json({apply:false});
    return;
  }
  else{

    connection.getConnection(function(err,conn){
      if(err){
        res.json({apply:false});
        throw err;
      }
      conn.query("select count(id) as cnt from accountlist where id='"+mid+"';",function(err, rows, fields){
        if(err){
          res.json({apply:false});
          throw err;
        }
        console.log("count sel");
        if(rows[0].cnt==0){
          conn.changeUser({user : 'logineduser',password:'logineduser123',database:'logindb'}, function(err) {
            if(err){
              res.json({apply:false});
              throw err;
            }
            var qryy = "insert into accountlist(accesslevel,id,pw) values(3,'"+mid+"','"+newpw+"');";
            conn.query(qryy,function(err,rows,fields){
              if(err){
                res.json({apply:false});
                throw err;
              }
              conn.commit(function(err){
                if(err){
                  res.json({apply:false});
                  throw err;
                }
                var qry2 = "insert into user_info(id,nickname) values('"+mid+"','"+nn+"');";
                conn.query(qry2,function(err,rows,fields){
                  if(err){
                    res.json({apply:false});
                    throw err;
                  }
                  conn.commit(function(err){
                    if(err){
                      res.json({apply:false});
                      throw err;
                    }
                    res.json({apply:true});
                  });
                });
              });
            });
          });
        }else{
          res.json({apply:false});
          throw err;
        }
      });
    });
  }
});
noteapp.get("/accountmgr/:id/duplicationcheck",function(req,res){
  const mid = req.params.id;
  console.log("dup chkid: "+mid);
  if(!checkinjectionquery(mid)){
  connection.query("select count(id) as cnt from accountlist where id='"+mid+"';",function(err, rows, fields){
    if(err){
      res.json({state:true});
      throw err;
    }
    if(rows[0].cnt>0){
      res.json({state:true});
    }else{
      res.json({state:false});
    }
  });
  }else{
    res.json({state:true});
  }

});
noteapp.get("/addons/naverrank.json",function(req,res){
  var url = "https://m.naver.com/";
  requestlib(url, function(error, response, html){  
    if (error) {res.json({value:"error"});throw error};
    var $ = cheerio.load(html);
   // console.log("cheerio loaded");
    const rst = [];
    $('script').each(function(){
        var hc = $(this).html();
        if(hc.includes("oRTK :")&&hc.includes("oHTP :")&&hc.indexOf("oRTK :")<hc.indexOf("oHTP :")){
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

noteapp.get("/addons/universitynotice.json",function(req,res){
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
noteapp.post("/:id/articles/pwdchk.json",function(req,res){
  console.log("acces");
  var mid = req.params.id;
  console.log("access /"+mid+"/articles/pwdchk.json");
  var mpw = req.body.artpw===undefined?req.body.artpw : base64.decode(req.body.artpw);
  var artid = Number(req.body.artid) ^ 12343210;
  var sess = req.session;
  if(mpw===undefined||mpw==""||mpw===""||mpw==null||sess.uid ===undefined || sess.upw ===undefined
  ||checkinjectionquery(mpw)||checkinjectionquery(mid)
  ){//password가 없거나 로그인이 안된경우
    res.json({cor:false});
  }else{
    connection.getConnection(function(err,conn){
      if(err){res.json({cor:false});throw err;}
      conn.changeUser(chgusrinfo,function(err){
        if(err){res.json({cor:false});throw err;}
        conn.query("select count(*) as cnt from memolist where boardnumber="+artid+" and id='"+mid+"';",function(err,rows,fields){
          if(err){res.json({cor:false});throw err;}
          if(rows[0].cnt>0){
            conn.query("select count(*) as cnt from memocontent where boardnumber="+artid+" and passwordlocked='"+mpw+"';",function(err,rows2,fields){
              if(err){res.json({cor:false});throw err;}
              if(rows2[0].cnt>0){
                res.json({cor:true});
              }else{
                res.json({cor:false});
              }
            });
          }else{
            res.json({cor:false});
          }
        });
      });
    });
    
  }
});
noteapp.delete("/dologin.json",function(req,res){
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
  console.log(request.query.id+"\n"+request.query.pw);
     tmpid = request.session.uid===undefined?  base64.decode(request.query.id):request.session.uid;
     tmppw = request.session.uid===undefined? base64.decode(request.query.pw):request.session.upw;
loop = false;
console.log(tmpid+"\n"+tmppw);
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
noteapp.get("/memotransac/:id/mtdlt.json",function(request,response){
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



noteapp.post("/memotransac/:id/mtut.json",function(request,response){
  const method = request.method;
  const id = request.params.id;
 console.log("!"+JSON.stringify(request.body)+"!");
 //console.log("!"+JSON.stringify(JSON.parse(request.body))+"!");
  const bpw = request.body.boardpw;
  const pid = request.body.postid;
  const memodata = base64.decode(base64.decode(base64.decode(request.body.mdt)));
  const title = base64.decode(request.body.title);
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
            if(bpw!=undefined && bpw!=null && bpw.trim() !=""){
              memodata = aes256.encrypt(bpw,memodata);
              conn.query("update memocontent set memodata='"+memodata+"' , passwordlocked='"+bpw+"' title='"+title+"', modify_date=NOW() where boardnumber="+pid+";",function(err,rows,fields){
                if(err){
                    try{response.json({state:false});}catch(err2){}
                  throw err;
                }
                conn.commit(function(err){
                  if(err){
                    response.json({state:false});
                    throw err;
                  }
                  response.json({state:true});
                });
               
              });
            }else{
              conn.query("update memocontent set memodata='"+memodata+"' ,passwordlocked=NULL ,title='"+title+"', modify_date=NOW() where boardnumber="+pid+";",function(err,rows,fields){
                if(err){
                    try{response.json({state:false});}catch(err2){}
                  throw err;
                }
                conn.commit(function(err){
                  if(err){
                    response.json({state:false});
                    throw err;
                  }
                  response.json({state:true});
                });
               
              });
            }
          }else{
              conn.query("insert into memolist(id) values('"+id+"');",function(err,rows,fields){
              console.log("insert pid ok");
              conn.commit(function(err){if(err){throw err;}
                console.log("commit of insert pid ok");
                conn.query("select boardnumber as bn from memolist where id='"+id+"' order by boardnumber desc limit 1;",function(err,rows,fields){
                  if(err){
                    try{response.json({state:false});}catch(err2){}
                    throw err;
                  }
                  console.log("select boardnumber ok");
                  if(bpw!=undefined && bpw != null && bpw.trim()!=""){
                   // memodata = aes256.encrypt(bpw,memodata);
                   var  encryptedmemodata = aes256.encrypt(bpw,memodata);
                    conn.query("insert into memocontent(boardnumber,passwordlocked,title,memodata) values("+rows[0].bn+", '"+bpw+"' , '"+title+"' , '"+encryptedmemodata+"');",function(err,rows2,fields){
                      if(err){
                        try{response.json({state:false});}catch(err2){}
                        throw err;
                      }
                      console.log("insert memocontent ok");
                      conn.commit(function(err){if(err){try{response.json({state:false});}catch(err2){}
                        throw err;}
                        response.json({state:true,pid:rows[0].bn});
                      });
                    });
                  }
                  else{
                    conn.query("insert into memocontent(boardnumber,title,memodata) values("+rows[0].bn+", '"+title+"' , '"+memodata+"');",function(err,rows2,fields){
                      if(err){
                        try{response.json({state:false});}catch(err2){}
                        throw err;
                      }
                      console.log("insert memocontent ok");
                      conn.commit(function(err){if(err){try{response.json({state:false});}catch(err2){}
                        throw err;}
                        response.json({state:true,pid:rows[0].bn});
                      });
                    });
                  }
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
noteapp.get("/memotransac/:id/mt.json",function(request,response){
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
          
          if(err){rst["succresp"]=false;
              response.json(rst);
                throw err;}
          console.log("open connection ok");
      conn.changeUser({user : 'logineduser',password:'logineduser123',database:'memodb'}, function(err) {
        if(err){rst["succresp"]=false;
              response.json(rst);
                throw err;}
        
          if(bpw!=undefined&&bpw!=null&&bpw.trim()!=''){
            var qrypostfix = "from memoview where id='"+id+"' and boardnumber="+pid+" and passwordlocked='"+bpw+"';"

            conn.query("select count(*) as cnt "+qrypostfix,function(err,rows2,fields){
              if(err){rst["succresp"]=false;
              response.json(rst);
                throw err;}
              if(rows2.cnt>0){
                conn.query("select * "+qrypostfix,function(err,rows,fields){
                if(err){rst["succresp"]=false;
              response.json(rst);
                throw err;}
                console.log("memo content quary1 ok(l: "+ rows.length);
                  rst["post_title"]=rows[0].title;
                  rst["content"]=rows[0].memodata.toString();
                  rst["content"]=aes256.decrypt(bpw,rst["content"]);
                  rst["succresp"]=true;
                  rst["modify_date"]= rows[0].modify_date;
                  response.json(rst);
                  //console.log(rst);
                });
            }
            else{
              rst["succresp"]=false;
              response.json(rst);
            }
            });
          }else{
            conn.query("select * from memoview where id='"+id+"' and boardnumber="+pid+";",function(err,rows,fields){
              if(err){rst["succresp"]=false;
              response.json(rst);
                throw err;}
              console.log("memo content quary2 ok(l:"+rows.length);
              rst["post_title"]=rows[0].title;
              rst["content"]=rows[0].memodata.toString();
              rst["modify_date"]= rows[0].modify_date.toString();
              rst["succresp"]=true;
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
//noteapp.post("/memotransac/:id/mt.json",function(request,response){});

noteapp.get("/:id/querydata.json",function(request,response){
  const id = request.params.id;
  //try{connection.connect(); }catch(err2){}
  const pid = request.query.postid;
  const bpw = request.query.boardpw;
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
  if(bpw!=undefined && bpw!=null && bpw.trim()!=""){
    conn.query("select boardnumber,title,memodata,modify_date as md from memoview where id='"+id+"' and boardnumber="+pid+" ;",function(err,rows,fields){
    if (err) throw err;
    const rst = {list_count:rows.length,data:[]};
    console.log("select pass and len="+rows.length);
    if(rows.length==0){
      rst["resp"] = 'data query is succeed ,but no data';
      response.json(rst);
    }else{
      for(i=0;i<rows.length;i++){

        rows[i]["memodata"]=base64.encode(base64.encode(aes256.decrypt( bpw,rows[i]["memodata"].toString())));
        rows[i]["title"]=base64.encode(rows[i]["title"]);
        rst["data"].push(rows[i]);
        //response.json(rows[i]);
        break;
        //rst.values.push();
      }
      response.json(rst);
    }
  });
  }else
  conn.query("select boardnumber,title,memodata,modify_date as md from memoview where id='"+id+"' and boardnumber="+pid+" ;",function(err,rows,fields){
    if (err) throw err;
    const rst = {list_count:rows.length,data:[]};
    console.log("select pass and len="+rows.length);
    if(rows.length==0){
      rst["resp"] = 'data query is succeed ,but no data';
      response.json(rst);
    }else{
      for(i=0;i<rows.length;i++){

        rows[i]["memodata"]=base64.encode(base64.encode(rows[i]["memodata"].toString()));
        rows[i]["title"]=base64.encode(rows[i]["title"]);
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



noteapp.get("/:id/inquirylist.json",function(request,response){
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
        if(rows[i].passwordlocked!=null&&rows[i].passwordlocked.trim()!=""){
          rows[i]["pwdreq"]=true;
        }
        else{
          rows[i]["pwdreq"]=false;
        }
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
noteapp.use('/dologin.json',lg);
//noteapp.use('/:id/inquirylist.json',queryMemoList);
// catch 404 and forward to error handler
noteapp.use(function(req, res, next) {
  var err = new Error('Not Found');
  err.status = 404;
  next(err);
});

// error handler
noteapp.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.noteapp.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});

module.exports = noteapp;
noteapp.listen(8080,function (){

});

