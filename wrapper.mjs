import brex from "./lib/binding.js";

/**
 * 
 * @param {string} pattern 
 * @param {string} input 
 * @returns {boolean}
 */
export function accepts(pattern, input) {
    return brex.accepts(pattern, input);
}

/**
 * @param {string} pattern
 * @param {string} input
 * @returns {boolean}
 */
export function startsWith(pattern, input) {
    return brex.startsWith(pattern, input);
}

/**
 * @param {string} pattern
 * @param {string} input
 * @returns {boolean}
 */
export function endsWith(pattern, input) {
    return brex.endsWith(pattern, input);
}

/**
 * @param {string} input
 * @returns {void}
 */
export function initializeLexer(input) {
    brex.initializeLexer(input);
}

/**
 * @param {string} pattern
 * @param {number} start
 * @returns {string | null}
 */
export function lexFront(pattern, start) {
    return brex.lexFront(pattern, start);
}

/**
 * @param {litstr} pattern
 * @returns {string | null}
 */
export function validateStringLiteral(litstr) {
    return brex.validateStringLiteral(litstr);
}

/**
 * @param {litstr} pattern
 * @returns {string | null}
 */
export function validateCStringLiteral(litstr) {
    return brex.validateCStringLiteral(litstr);
}