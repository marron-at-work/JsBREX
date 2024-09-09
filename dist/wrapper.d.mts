/**
 * @param {any[]} rsystem
 * @returns {string[] | null}
 * @throws {TypeError}
 */
export function loadConstAndValidateRESystem(rsystem: any[]): string[] | null;
/**
 * @param {string} litstr
 * @returns {string}
 * @throws {TypeError}
 */
export function validateStringLiteral(litstr: string): string;
/**
 * @param {string} litstr
 * @returns {string}
 * @throws {TypeError}
 */
export function validateCStringLiteral(litstr: string): string;
/**
 *
 * @param {string} pattern
 * @param {string} input
 * @returns {boolean}
 * @throws {TypeError}
 */
export function accepts(pattern: string, input: string): boolean;
/**
 * @param {string} pattern
 * @param {string} input
 * @returns {boolean}
 * @throws {TypeError}
 */
export function startsWith(pattern: string, input: string): boolean;
/**
 * @param {string} pattern
 * @param {string} input
 * @returns {boolean}
 * @throws {TypeError}
 */
export function endsWith(pattern: string, input: string): boolean;
/**
 * @param {string} input
 * @returns {void}
 * @throws {TypeError}
 */
export function initializeLexer(input: string): void;
/**
 * @param {string} pattern
 * @param {number} start
 * @returns {string | null}
 * @throws {TypeError}
 */
export function lexFront(pattern: string, start: number): string | null;
//# sourceMappingURL=wrapper.d.mts.map