const Jsbrex = require("../lib/binding.js");
const assert = require("assert");

assert(Jsbrex, "The expected function is undefined");

console.log(Jsbrex("hello", "hello"));
console.log(Jsbrex("bob", "nope"));

console.log("Tests passed- everything looks OK!");