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
 * @returns {string | null}
 */
export function lexFront(pattern: string, start: number): string | null;
/**
 * @param {litstr} pattern
 * @returns {string | null}
 */
export function validateStringLiteral(litstr: any): string | null;
/**
 * @param {litstr} pattern
 * @returns {string | null}
 */
export function validateCStringLiteral(litstr: any): string | null;
/**
 * @param {any[]} rsystem
 * @returns {string[] | null}
 */
export function loadConstAndValidateRESystem(rsystem: any[]): string[] | null;
/**
 * @param {string} pattern
 * @param {string} input
 * @param {boolean} isunicode
 * @returns {boolean}
 */
export function runNamedRegexAccepts(pattern: string, input: string, isunicode: boolean): boolean;
//# sourceMappingURL=wrapper.d.mts.map