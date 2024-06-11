/**
 *
 * @param {string} pattern
 * @param {string} input
 * @returns {boolean}
 */
export function accepts(pattern: string, input: string): boolean;
/**
 * @param {string} pattern
 * @param {string} input
 * @returns {boolean}
 */
export function startsWith(pattern: string, input: string): boolean;
/**
 * @param {string} pattern
 * @param {string} input
 * @returns {boolean}
 */
export function endsWith(pattern: string, input: string): boolean;
/**
 * @param {string} input
 * @returns {void}
 */
export function initializeLexer(input: string): void;
/**
 * @param {string} pattern
 * @param {number} start
 */
export function lexFront(pattern: string, start: number): any;
//# sourceMappingURL=wrapper.d.mts.map