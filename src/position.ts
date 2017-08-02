import { CenterOptions } from './interfaces/index';
import { arrayUtil } from './util/array';
import * as Bindings from 'bindings';
const Center = Bindings('center');
const TSP = Bindings('tsp');

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
    startIndex: 0
  };

  private cost(points: Array<Array<number>>, ...args: Array<number>);
  /**
   * Summates the Pythagorean distance between a center and an arbitrary amount
   * of points as an efficient and powerful cost function.
   *
   * @function
   * @param {Array} points 2D Array of points on a plane
   * @param {number} x x coordinate of center
   * @param {number} y y coordinate of center
   * @return {number} Sum of Pythagorean distances from center to each point
   */
  private cost(points: Array<Array<number>>, x: number, y: number): number {
    return points.reduce(
      (sum, value) =>
        sum + Math.sqrt((value[0] - x) ** 2 + (value[1] - y) ** 2),
      0
    );
  }

  /**
   * Finds the center of mass of an arbitrary amount of points using standard
   * median.
   *
   * @function
   * @param {Array} points 2D Array of points on a plane
   * @return {object} Center of mass
   */
  private mass(
    points: Array<Array<number>>
  ): { center: Array<number>; score: number } {
    const center = [
      points.reduce((sum, value) => sum + value[0], 0) / points.length,
      points.reduce((sum, value) => sum + value[1], 0) / points.length
    ];
    return { center: center, score: this.cost(points, ...center) };
  }

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
   * @desc heavily optimized JS version of @see
   * @function
   * @return {Array} Geometric center of the Position
   */
  get center(): Array<number> {
    return Center.geometric(
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
    return this.mass(this.locations).center;
  }

  /**
   * Returns the index order of the least-costly path between all locations on
   * the plane by solving the TSP.
   *
   * @name Position#path
   * @function
   * @return {Array} Order of indeces of the locations on the plane that gives
   * the shortest path
   */
  get path(): Array<number> {
    return TSP.tsp(this.locations, this.options.startIndex);
  }

  /**
   * Calculates the decimal improvement of Position#center as compared to
   * Position#median in each dimension.
   *
   * @name Position#score
   * @function
   * @return {Array} Median of the Position
   */
  get score(): number {
    const [median, center] = [
      this.mass(this.locations).score,
      Center.geometric(
        this.locations,
        this.options.subsearch,
        this.options.epsilon,
        this.options.bounds
      ).score
    ];
    return (median - center) / median;
  }
}
