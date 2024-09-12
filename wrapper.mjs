import brex from "./lib/binding.js";

/**
 * @param {any[]} rsystem
 * @returns {string[] | null}
 * @throws {TypeError}
 */
export function loadConstAndValidateRESystem(rsystem) {
    return brex.loadConstAndValidateRESystem(rsystem);
}

/**
 * @param {string} litstr
 * @returns {string}
 * @throws {TypeError}
 */
export function validateStringLiteral(litstr) {
    return brex.validateStringLiteral(litstr);
}

/**
 * @param {string} litstr
 * @returns {string}
 * @throws {TypeError}
 */
export function validateCStringLiteral(litstr) {
    return brex.validateCStringLiteral(litstr);
}

/**
 * 
 * @param {string} pattern 
 * @param {string} input 
 * @param {string} inns 
 * @returns {boolean}
 * @throws {TypeError}
 */
export function accepts(pattern, input, inns) {
    return brex.accepts(pattern, input, inns);
}

/**
 * @param {string} pattern
 * @param {string} input
 * @param {string} inns 
 * @returns {boolean}
 * @throws {TypeError}
 */
export function startsWith(pattern, input, inns) {
    return brex.startsWith(pattern, input, inns);
}

/**
 * @param {string} pattern
 * @param {string} input
 * @param {string} inns 
 * @returns {boolean}
 * @throws {TypeError}
 */
export function endsWith(pattern, input, inns) {
    return brex.endsWith(pattern, input, inns);
}

/**
 * @param {string} input
 * @returns {void}
 * @throws {TypeError}
 */
export function initializeLexer(input) {
    brex.initializeLexer(input);
}

/**
 * @param {string} pattern
 * @param {string} input
 * @returns {boolean}
 * @throws {TypeError}
 */
export function lexAccepts(pattern, input) {
    return brex.lexFront(pattern, start);
}

/**
 * @param {string} pattern
 * @param {number} start
 * @returns {string | null}
 * @throws {TypeError}
 */
export function lexFront(pattern, start) {
    return brex.lexFront(pattern, start);
}

