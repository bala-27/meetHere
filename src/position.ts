import { CenterOptions } from './interfaces/index';
import { arrayUtil } from './util/array';
import * as Bindings from 'bindings';
const CENTER = Bindings('center');
const POLYNOMIAL = Bindings('polynomial');
const TSP = Bindings('tsp');
const Method = {
  tsp: 116,
  naiveVsp: 110
};

arrayUtil();

/**
 * A prototype describing a set of points on a plane.
 *
 * ```
 * import { Position } from 'meethere';
 *
 * const options = {
 *   subsearch: true,
 *   epsilon: 1e-4,
 *   bounds: 15
 * }
 *
 * let Plane = new Position(
 *   [ [5, 6.1], [-2.07, -1.33], [9.8, -0.02] ],
 *   options
 * );
 * Plane.add([1, -4]);
 * Plane.center // => [ 2.2460648128951566, -0.853617418121833 ]
 * Plane.score // => 0.010113270070291593
 * ```
 *
 * @class
 */
export class Position {
  locations: Array<Array<number>>;
  options: CenterOptions;

  /**
   * Default geometric center options
   *
   * @constant
   * @type {CenterOptions}
   * @default
   */
  static defaultCenterOptions: CenterOptions = {
    subsearch: false,
    epsilon: 1e-3,
    bounds: 10,
    startIndex: 0,
    degree: null
  };

  /**
   * Creates a Position on a plane described by a set of locations.
   *
   * @constructs
   * @param {Array} locations 2D Array of points on a plane
   * @param {CenterOptions} [options=Position.defaultCenterOptions] General search options
   */
  constructor(locations: Array<Array<number>>, options: CenterOptions = {}) {
    this.locations = locations;
    this.options = { ...Position.defaultCenterOptions, ...options };
  }

  /**
   * Adds a location to the set of points.
   *
   * @name Position#add
   * @function
   * @param {Array} location Point to add
   */
  add(location: Array<number>): void {
    this.locations.push(location);
  }

  /**
   * Removes a location to the set of points.
   *
   * @name Position#remove
   * @function
   * @param {Array} location Point to remove
   * @return {Array|number} The removed location, or `-1` if no match is found
   */
  remove(location: Array<number>): Array<number> | number {
    const idx = this.locations.deepIndexOf(location);
    if (idx > -1) {
      return this.locations.splice(idx, 1)[0];
    }
    return idx;
  }

  /**
   * Replaces an existing location.
   *
   * @name Position#adjust
   * @function
   * @param {Array} location Point to adjust
   * @param {Array} to Value to adjust to
   * @return {Array|number} The previous location, or `-1` if no match is found
   */
  adjust(location: Array<number>, to: Array<number>): Array<number> | number {
    const idx = this.locations.deepIndexOf(location);
    if (idx > -1) {
      return this.locations.splice(idx, 1, to)[0];
    }
    return idx;
  }

  /**
   * Calculates the geometric center of the Position.
   *
   * @name Position#center
   * @see https://stackoverflow.com/a/12934484
   * @desc heavily optimized, natively binded version of @see
   * @function
   * @return {Array} Geometric center of the Position
   */
  get center(): Array<number> {
    return CENTER.geometric(
      this.locations,
      this.options.subsearch,
      this.options.epsilon,
      this.options.bounds
    ).center;
  }

  /**
   * Calculates the median (center of mass) of the Position.
   *
   * @name Position#median
   * @desc a rudimentary estimate for Position#center
   * @function
   * @return {Array} Geometric center of the Position
   */
  get median(): Array<number> {
    return CENTER.mass(this.locations).center;
  }

  /**
   * Returns the index order of the least-costly path between all locations on
   * the plane through a rudimentary solution of the TSP (~80 point efficiency).
   *
   * @name Position#path
   * @TODO More involved TSP solution (figure out or-tools bindings)
   * @function
   * @return {Array} Order of indeces of the locations on the plane that gives
   * the shortest path
   */
  get path(): Array<number> {
    return TSP.tsp(this.locations, this.options.startIndex, Method['tsp']);
  }

  /**
   * Returns the coefficients of a n-degree polynomial best-fit to the locations
   * on the plane. Degree is specified during class instantiation, and is auto-
   * calculated by default.
   *
   * @name Position#polynomial
   * @function
   * @return {Array} an n-length array of the coefficients of a best-fit,
   * n-degree polynomial, where each index corresponds to its degree. E.g.:
   * ```
   * [1, 4, 9] // => 1(x^0) + 4(x^1) + 9(x^2)
   * ```
   */
  get polynomial(): Array<number> {
    return POLYNOMIAL.bestFit(this.locations, this.options.degree);
  }

  /**
   * Returns the index order of the least-costly manhattan-style drive between
   * all locations on the plane through a naive solution of the VRP (~80 point
   * efficiency).
   *
   * @name Position#naiveDrive
   * @TODO More involved VRP solution (figure out or-tools bindings)
   * @function
   * @return {Array} Order of indeces of the locations on the plane that gives
   * the shortest manhattan path
   */
  get naiveDrive() {
    return TSP.tsp(this.locations, this.options.startIndex, Method['naiveVsp']);
  }

  /**
   * Calculates the net cost of travelling from the points to their median.
   *
   * @name Position#medianCost
   * @function
   * @return {number} Cost of travelling
   */
  get medianCost(): number {
    return CENTER.mass(this.locations).score;
  }

  /**
   * Calculates the net cost of travelling from the points to their geometric
   * center.
   *
   * @name Position#medianCost
   * @function
   * @return {number} Cost of travelling
   */
  get centerCost(): number {
    return CENTER.geometric(
      this.locations,
      this.options.subsearch,
      this.options.epsilon,
      this.options.bounds
    ).score;
  }

  /**
   * Calculates the percent improvement of Position#center as compared to
   * Position#median in each dimension.
   *
   * @name Position#score
   * @function
   * @return {Array} Median of the Position
   */
  get score(): number {
    const [median, center] = [this.medianCost, this.centerCost];
    return (median - center) / median;
  }
}

export { Bindings };
