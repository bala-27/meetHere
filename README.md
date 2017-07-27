# meetHere <img src="./meetHere.svg" style="height:1.5em"/>
> A hyper-efficient library for position manipulation.

meetHere is a componentization of a past endeavor of mine. It allows for quick,
elegant manipulation of positional data on a 2D plane, and comes with
first-class Google Maps support for on-the-fly determination of locations
relevant to you and your users. Prominent features include:
* High-precision geometric center calculations
* Positional center scoring
* Determining relevant nearby locations
* Powerful async operations
* First-class TypeScript support
- [ ] C++ bindings (coming soon)
- [ ] Time-minimum center (coming soon)

## Installation
```bash
$ yarn add meetHere
```

```bash
$ npm i meetHere --save
```

## Usage
You can find the full API [here]()

Currently, two classes are available:

#### `Position` - a blazing fast, 2D-locations manipulations prototype.
> For general purpose apps, like physics libraries or games.

```
import { Position } from 'meetHere';

let Plane = new Position([[0, 1], [1.5, 3], [-9, 1.07]]);
Plane.add([3, 4]);
Plane.center # => [ 1.4990254494359172, 2.999397294475018 ]
Plane.median # => [ -1.125, 2.2675 ]
Plane.score # => 0.11155562920022742
```

#### `MeetHere` - a subclass of `Position` with first-class Google Maps support.
> For user-interactive applications, like meetHere itself.

To use non-super methods of `MeetHere`, you'll need a Google Maps API key, as
described
[here](https://github.com/googlemaps/google-maps-services-js#api-keys).

```
import { MeetHere } from 'meetHere';

const user = [33.0952311, -96.8640427];
const west = [33.0437115, -96.8157956];
const senior = [33.0284505, -96.7546927];

let Map = new MeetHere(
  [user, west, senior],
  'MY_GOOGLE_MAPS_TOKEN'
);
Map.meetHere # => [ 33.04371181611578, -96.81579457575877 ]
Map.nearby().then(console.log) # => { results: [...] }
```

## License
MIT, &copy; hafiz 2017
