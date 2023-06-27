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
    res.render("FinalProj_Edit", {pageTitle: "Edit Phase Duration", item: phase_data});
});

router.post('/Edit', async (req, res) => {
    const modelData = {
        phase1_duration: req.body.Phase1,
        phase2_duration: req.body.Phase2,
        phase3_duration: req.body.Phase3,
        phase4_duration: req.body.Phase4
    };
    let phase_data = await phaseModel.findByIdAndUpdate(req.query.id, modelData);
    res.redirect("/R/List");
});

//--- Define endpoint for the API: ------------
router.get('/R/:phase', async (req, res) => {
    // Get the "phase" parameter from the URL (as appears in the line above: .../R/phaseN_duration)
    const myPhase = req.params.phase;
    
    // Query the collection for the specific document:
    // Find a line ("document") #1 - the only one line in the table.
    // In a more complicated scenario, line_number to be set dynamically.
    // Fetch only the duration value of the required Phase:
    const document = await phaseModel.findOne({ line_number: 1 }, myPhase);

    // Extract the desired field value from the Mongo document (line).
    // If not found, default value will be "2".
    const fieldValue = document ? document.fieldName : 2;
       
    // Return the response as a single value (integer)
    res.send(fieldValue);
  });
