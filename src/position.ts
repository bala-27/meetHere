import { Center } from './center';

/**
 * Describes a set of points on a plane.
 * @class
 */
export class Position {
  locations: Array<Array<number>>;
  subsearch: boolean;
  epsilon: number;
  bounds: number;

  /**
   * Creates a Position on a plane described by a set of points.
   *
   * @constructs
   * @param {Array} locations 2D Array of points on a plane
   * @param {boolean} [subsearch=false] Whether to search for centroid obliquely
   * @param {number} [epsilon=1e-3] Precision for centroid calculations
   * @param {number} [bounds=10] Starting unit bounds for centroid calculations
   */
  constructor(
    locations: Array<Array<number>>,
    subsearch: boolean = false,
    epsilon: number = 1e-3,
    bounds: number = 10
  ) {
    this.locations = locations;
    this.subsearch = subsearch;
    this.epsilon = epsilon;
    this.bounds = bounds;
  }

  /**
   * Adds a location to the set of points.
   *
   * @name Position#add
   * @function
   * @param {Array} location 2D points on a plane
   */
  add(location: Array<number>): void {
    this.locations.push(location);
  }

  /**
   * Removes a location to the set of points.
   *
   * @name Position#remove
   * @function
   * @param {Array} location 2D points on a plane
   * @return {Array|number} The removed location, or `-1` if no match is found
   */
  remove(location: Array<number>): Array<number> | number {
    for (let i = 0; i < this.locations.length; i++) {
      if (
        this.locations[i][0] === location[0] &&
        this.locations[i][1] === location[1]
      ) {
        this.locations.splice(i, 1);
        return location;
      }
    }
    return -1;
  }

  /**
   * Calculates the geometric center of the Position.
   *
   * @name Position#center
   * @see Center#geometric
   * @function
   * @return {Array} Geometric center of the Position
   */
  get center(): Array<number> {
    return Center.geometric(...this.values).center;
  }

  /**
   * Calculates the median (center of mass) of the Position.
   *
   * @name Position#median
   * @see Center#mass
   * @desc a rudimentary estimate for Position#center
   * @function
   * @return {Array} Geometric center of the Position
   */
  get median(): Array<number> {
    return Center.mass(this.locations).center;
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
      Center.mass(this.locations).score,
      Center.geometric(...this.values).score
    ];
    return (median - center) / median;
  }

  /**
   * Returns the values of each key of the current Position object
   *
   * @function
   * @private
   * @return {Array} Values of each key of the Position
   */
  private get values(): Array<Array<number> | number | boolean> {
    return Object.getOwnPropertyNames(this).map(v => this[v]);
  }
}
