const express = require("express");
const router = express.Router();
module.exports = router;

const itemModel = require("../models/modelItem");
const categModel = require("../models/modelCategories");

router.get('/Add', async(req, res) => {
    let cat_data = await categModel.find();
    res.render("ItemAdd", {pageTitle: "Add Item", 
        allCats: cat_data, 
        item: {}});
});

router.post('/Add', (req, res) => { 
    if (req.body.newCat !== "") {
        const catData = new categModel({categoryName: req.body.newCat});
        catData.save();
        res.redirect("/I/Add");
    } else {
        const modelData = new itemModel({
            itemName: req.body.shop_item,
            category: req.body.category
        });
        modelData.save();
        //res.redirect("/I/List");
        res.send('Saved');
    }
});
/*
router.get('/List', async (req, res) => {
    let cat_data = await itemModel.find();
    res.render("CategoryList", {pageTitle: "Categories Management", data: cat_data});
});

router.get('/Edit', async (req, res) => {
    let item_data = await itemModel.findById(req.query.id);
    res.render("CategoryAdd", {pageTitle: "Edit Category", item: item_data});
});

router.post('/Edit',async (req, res) => {
    const modelData = {categoryName:req.body.categoryName};
    let item_data = await itemModel.findByIdAndUpdate(req.query.id, modelData);
    res.redirect("/I/List");
});

router.post('/Delete', async (req, res) => {
    let item_data = await itemModel.findByIdAndDelete(req.body.id);
    res.redirect("/I/List");
});

*/

