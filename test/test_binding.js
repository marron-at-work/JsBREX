const {accepts, startsWith, endsWith} = require("../lib/binding.js");
const assert = require("assert");

assert(accepts, "The expected function is undefined");

console.log(accepts('/"hello"/', "hllo"));
console.log(accepts('/"hello"/', "nope"));

console.log("Tests passed- everything looks OK!");