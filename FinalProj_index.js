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

//--- Connecting router ------------------------
const phases_rtr = require('./FinalProj_router');
app.use('/R', phases_rtr);

//------------------------------------------------
app.listen(port, () => {            //server starts listening for any attempts from a client to connect at port: {port}
    console.log(`Now listening on port ${port}`);
});
