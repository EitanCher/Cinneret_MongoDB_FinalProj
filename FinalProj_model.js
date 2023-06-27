const mongoose = require("mongoose");

const dataSchema = new mongoose.Schema({
    line_number: {
        required: true,
        type: Number
    },
    phase1_duration: {
        required: true,
        type: Number/*,
        min: 2*/
    },
    phase2_duration: {
        required: true,
        type: Number/*,
        min: 2*/
    },
    phase3_duration: {
        required: true,
        type: Number/*,
        min: 2*/
    },
    phase4_duration: {
        required: true,
        type: Number/*,
        min: 2*/
    }
})

module.exports = mongoose.model('FinalProj_Phases', dataSchema)
