import { Position } from './position';
import { createClient } from '@google/maps';

/**
 * Describes a Google Maps client
 *
 * @interface
 */
interface GoogleMapsClient {
  directions: Function;
  distanceMatrix: Function;
  elevation: Function;
  elevationAlongPath: Function;
  geocode: Function;
  geolocate: Function;
  reverseGeocode: Function;
  places: Function;
  placesNearby: Function;
  placesRadar: Function;
  place: Function;
  placesPhoto: Function;
  placesAutoComplete: Function;
  placesQueryAutoComplete: Function;
  snapToRoads: Function;
  nearestRoads: Function;
  speedLimits: Function;
  snappedSpeedLimits: Function;
  timezone: Function;
}

/**
 * Describes MeetHere#nearby options
 *
 * @interface
 */
interface PlacesOptions {
  language?: string;
  radius?: number;
  keyword?: string;
  minprice?: number;
  maxprice?: number;
  name?: string;
  opennow?: boolean;
  rankby?: string;
  type?: string;
  pagetoken?: string;
}

/**
 * Describes a set of points on a map.
 * @class
 * @extends Position
 */
export class MeetHere extends Position {
  client: GoogleMapsClient;

  /**
   * Default nearby search options
   *
   * @constant
   * @type {object}
   * @default
   */
  static defaultSearchOptions = {
    language: 'English',
    rankby: 'distance'
  };

  /**
   * Creates a MeetHere on a map described by a set of points.
   *
   * @constructs
   * @param {Array} locations 2D Array of points on a map
   * @param {string} token Google Maps API token
   * @param {boolean} [subsearch=false] Whether to search for centroid obliquely
   * @param {number} [epsilon=1e-6] Precision for centroid calculations
   * @param {number} [bounds=10] Starting unit bounds for centroid calculations
   */
  constructor(
    locations: Array<Array<number>>,
    token: string,
    subsearch: boolean = false,
    epsilon: number = 1e-6,
    bounds: number = 10
  ) {
    super(locations, subsearch, epsilon, bounds);
    this.client = createClient({ key: token, Promise: Promise });
  }

  /**
   * Calculates the geometric center of the Position.
   *
   * @name MeetHere#meetHere
   * @alias Position#center
   * @function
   * @return {Array} Geometric center of the Position
   */
  get meetHere(): Array<number> {
    return super.center;
  }

  /**
   * Returns roads near the center of the MeetHere.
   *
   * @name MeetHere#roads
   * @function
   * @param {boolean} [geometric=true] Whether to use geometric/median center
   * @return {Promise} A Promise that will yield nearby roads or an error
   */
  roads(geometric: boolean = true): Promise<object> {
    const center = this.middle(geometric);
    return this.client
      .nearestRoads({ points: [center] })
      .asPromise()
      .then(response => response.json)
      .catch(error => error.json);
  }

  /**
   * Returns places near the center of the MeetHere.
   *
   * @name MeetHere#nearby
   * @see https://googlemaps.github.io/google-maps-services-js/docs/GoogleMapsClient.html#placesNearby
   * @function
   * @param {PlacesOptions} [options=defaultSearchOptions] Options to apply to search
   * request, can be any of @see
   * @param {boolean} [geometric=true] Whether to use geometric/median center
   * @return {Promise} A Promise that will yield nearby places or an error
   */
  nearby(
    options: PlacesOptions = MeetHere.defaultSearchOptions,
    geometric: boolean = true
  ): Promise<object> {
    options['location'] = this.middle(geometric);
    return this.client
      .placesNearby(options)
      .asPromise()
      .then(response => response.json)
      .catch(error => error.json);
  }

  /**
   * Returns the center of the MeetHere based on the geometric parameter.
   *
   * @function
   * @private
   * @param {boolean} geometric Whether to use geometric/median center
   * @return {Array} The center of the MeetHere
   */
  private middle(geometric: boolean): Array<number> {
    return geometric ? super.center : super.median;
  }
}
