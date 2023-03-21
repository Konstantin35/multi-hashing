
/*
 *
 * Algorithms (Updated)
 *
 */

const x17 = require('../../index').x17;

////////////////////////////////////////////////////////////////////////////////

describe('Test algorithm functionality', () => {

  // Deterministic
  test('Test implemented x17 algorithm', () => {
    const start = Buffer.from('000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f');
    const main = Buffer.from('75856e3aa9163eb0a6b57a196f64d5e82785b26e21d99b9ac287c9b6d5a24e8d', 'hex');
    const output = Buffer.from('c95715e1d013fe9f7d163441fe1c5b5899fcb94566aefb7a30bf350dd1f3d747', 'hex');
    expect(x17.apply(null, [start, main]).length).toBe(32);
    expect(x17.apply(null, [start, main])).toStrictEqual(output);
  });
});
