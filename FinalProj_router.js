const express = require("express");
const router = express.Router();
module.exports = router;

const phaseModel = require("./FinalProj_model");

router.get('/List', async (req, res) => {
    let phase_data = await phaseModel.find();
    res.render("FinalProj_List", {pageTitle: "Phases Management", data: phase_data});
});

router.get('/Edit', async (req, res) => {
    let phase_data = await phaseModel.findById(req.query.id);
    res.render("FinalProj_Edit", {pageTitle: "Edit Phase", item: phase_data});
});

router.post('/Edit', async (req, res) => {
    const modelData = {
        phase_number:   req.body.phaseNum,
        phase_name:     req.body.phaseName,
        phase_duration: req.body.phaseDuration
    };

    let phase_data = await phaseModel.findByIdAndUpdate(req.query.id, modelData);
    res.redirect("/R/List");
});

router.post('/Delete',async (req, res) => {
    let item_data = await phaseModel.findByIdAndDelete(req.body.id);
    res.redirect("/R/List");
});

router.get('/Add', (req, res) => {
    res.render("FinalProj_Edit", {pageTitle: "Add Phase", item:{} });
});

router.post('/Add',(req, res) => {
    const modelData = new phaseModel({
        phase_number:   req.body.phaseNum,
        phase_name:     req.body.phaseName,
        phase_duration: req.body.phaseDuration
    });
    modelData.save();
    res.redirect("/R/List");
});


//--- Define endpoint for the API: ------------
router.get('/:phase', async (req, res) => {
    // Get the ":phase" parameter from the URL
    const myPhase = req.params.phase;
    
    // Query the collection for the specific document:
    const myItem = await phaseModel.findOne({ phase_number: myPhase }, 'phase_duration');

    // Extract the desired field value from the Mongo document (line).
    const myValue = myItem.phase_duration;

    // If not found, default value will be "2".
    const myOutput = myValue ? myValue : 2;
 
    // Return the response as a single value (integer)
    res.send(myOutput.toString());
});
