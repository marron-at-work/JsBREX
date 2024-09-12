import {accepts, startsWith, endsWith, initializeLexer, lexFront, validateStringLiteral, validateCStringLiteral, loadConstAndValidateRESystem} from "../wrapper.mjs";
import assert from "node:assert";

assert(accepts, "The expected function is undefined");
assert(startsWith, "The expected function is undefined");
assert(endsWith, "The expected function is undefined");

assert(accepts('/"hello"/', "hello", "Main"));
assert(!accepts('/"hello"/', "nope", "Main"));

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

assert(validateStringLiteral, "The expected function is undefined");
assert(validateStringLiteral("hello") === "hello");
assert(validateStringLiteral("a🌵c") === "a🌵c");
assert(validateStringLiteral("a%x1f335;c") === "a🌵c");

assert(validateCStringLiteral, "The expected function is undefined");
assert(validateCStringLiteral("hello") === "hello");
assert(validateCStringLiteral("%x59;") === "Y");

assert.throws(() => validateCStringLiteral("a🌵c"));

const nsinfo1 = {
    "nsinfo": {
        "inns": "Main",
        "nsmappings": []
    },
    "reinfos": [
        {
            "name": "Foo",
            "restr": '/"abc"/'
        
        },
        {
            "name": "Bar",
            "restr": '/"xyz"/'
        
        },
        {
            "name": "Baz",
            "restr": '/${Foo} "-" ${Bar}/'
        }
    ]
};

const errors1 = loadConstAndValidateRESystem([nsinfo1]);
assert(errors1 === null);

assert(accepts("/${Main::Foo}/", "abc", "Main"));
assert(accepts("/${Main::Baz}/", "abc-xyz", "Main"));

assert(accepts("/${Foo}/", "abc", "Main"));

console.log("Tests passed- everything looks OK!");