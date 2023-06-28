const mongoose = require("mongoose");

const dataSchema = new mongoose.Schema({
    phase_number: {
        required: true,
        type: Number
    },
    phase_name: {
        required: true,
        type: String
    },
    phase_duration: {
        required: true,
        type: Number/*,
        min: 2*/
    }
})

module.exports = mongoose.model('FinalProj_Phases', dataSchema)
