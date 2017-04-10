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
var mysql      = require('mysql');
var connection = mysql.createPool({
  connectionLimit : 10,
  host     : 'localhost',
  user     : 'guest',
  password : '',
  database: 'logindb'
});
// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'jade');
app.use(session({
  resave:false,
  secret: '#$%^&!@memoapp&*()',
  saveUninitialized:true
}));
// uncomment after placing your favicon in /public
//app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', index);
app.use('/users', users);
var lg = express.Router().get("/",function(request,response){
connection.query("use logindb;",function(err,rows,fields){});
  //try{connection.connect(); }catch(err2){}

connection.query('SELECT * from availableloginlist;', function(err, rows, fields) {//1차적인 sql injection 방어
  if (err) throw err;
  //if(err)return;
  const rst = {};

  for(i=0;i<rows.length;i++){
    const chkid = request.query.id;
    const pw = request.query.pw;
    if(chkid!=null&&pw!=null&&pw.trim()!=''&&chkid.trim()!=''&&!(pw.includes("'")|| pw.includes('"'))&&!(chkid.includes("'")|| chkid.includes('"'))&&rows[i].pw===pw&&rows[i].id===chkid){
      rst["logined"]=true;
      const timestamp =  new Date().getTime();
      const expirets = timestamp+60*60*12*1000;
      var chkqur ="SELECT count(*) as cnt from loginhash where id='"+chkid+"' and hashv=md5('"+request.sessionID+"') ;";
      console.log("cnt qry: "+chkqur);
      connection.query(chkqur,function(err2,rows2,fields2){
        if (err2) {
          console.log("error at query");
          throw err2;
        }
        console.log(rows2[0].cnt);
        if(rows2[0].cnt ==0){
          console.log("before getconnection");
          connection.getConnection(function(err,conn){
          conn.beginTransaction(function(err){
            
            if(err){
              console.log("error at beginTrans");
              throw err;
            }
            console.log("pass begin transaction.");
          var mQuery = "insert into loginhash(id,hashv) values('"+chkid+"', md5('"+(request.sessionID)+"') "+");";
          console.log(mQuery);
          conn.query(mQuery,function(err3, rows3, fields3) {
            conn.commit(function(err){
            if(err){
              conn.rollback(function(){
                console.log("error at rollback");
                throw err;
              });
            }
          });
          });
          });
          //connection.query("commit;",function(err3, rows3, fields3) {});
        });
        
        }
        /*var mQuery2 = "SELECT hashv from loginhash where hashv=md5('"+request.sessionID+"') and id='"+ chkid +"';";
        console.log(mQuery2);
        connection.query(mQuery2,function(err3,rows3,fields3){
          if (err3) throw err3;
          if(rows3!=null){
          console.log(rows3);
          //rst["hashd"]=rows3[0].hashv;
          }
        });*/
      });
      
      break;
    }
  }
  response.json(rst);
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
