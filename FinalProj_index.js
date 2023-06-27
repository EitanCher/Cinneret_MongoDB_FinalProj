const express = require('express');
const mongoose = require('mongoose');

const port = 6655;
const app = express();

app.use(express.json());        // enable working with jason
app.set("view engine", "ejs");  // enable rendering engine
var bodyParser = require('body-parser');
app.use(bodyParser.urlencoded({extended: false}));

const path = require('path');
app.use(express.static(path.join(__dirname, "css")));

//------------------------------------------------------
require('dotenv').config();

mongoose.connect(process.env.mongoString);
const database = mongoose.connection
database.on('error', (error) => {
    console.log(error)
})

database.once('connected', () => {
    console.log('Database Connected');
})

//--- Connecting routers ------------------------
const rooms_rtr = require('./routers/routRoom');
app.use('/R', rooms_rtr);
const calls_rtr = require('./routers/routCalls');
app.use('/C', calls_rtr);
const api_rtr = require('./routers/routAPI');
app.use('/API', api_rtr);
//------------------------------------------------
app.listen(port, () => {            //server starts listening for any attempts from a client to connect at port: {port}
    console.log(`Now listening on port ${port}`);
});
