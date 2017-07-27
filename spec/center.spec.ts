import { Center } from '../src/center';
import { expect } from 'chai';
import 'mocha';

describe('Center', () => {
  describe('works with default parameters', () => {
    const test = Center.geometric([[1, 2], [5, 6.6], [-7, 8.1], [3.1, -1.7]]);
    expect(test).to.have
      .property('center')
      .that.deep.equals([0.9998479030807611, 2.000560357070879]);
    expect(test).to.have.property('score').that.equals(20.41098713341468);
  });
});
