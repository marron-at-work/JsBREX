const {accepts, startsWith, endsWith, initializeLexer, lexFront} = require("../lib/binding.js");
const assert = require("assert");

assert(accepts, "The expected function is undefined");
assert(startsWith, "The expected function is undefined");
assert(endsWith, "The expected function is undefined");

assert(accepts('/"hello"/', "hello"));
assert(!accepts('/"hello"/', "nope"));

assert(initializeLexer, "The expected function is undefined");
assert(lexFront, "The expected function is undefined");

initializeLexer("hello world");
assert(lexFront('/"hello"/', 0) === "hello");
assert(lexFront('/"hello"/', 3) === null);
assert(lexFront('/"orld"/', 7) === "orld");
assert(lexFront('/"orld!"/', 7) === null);


initializeLexer("hello 42n world");
assert(lexFront('/[0-9]+"n"/', 6) === "42n");
assert(lexFront('/[0-9]+"n"/', 7) === "2n");
assert(lexFront('/[0-9]+"n"/', 8) === null);

console.log("Tests passed- everything looks OK!");