
# CHash

Create a perceptual hash of an image

[Perceptual hashing](https://en.wikipedia.org/wiki/Perceptual_hashing)

> Perceptual hashing is the use of an algorithm that produces a snippet
> or fingerprint of various forms of multimedia. Perceptual hash
> functions are analogous if features are similar, whereas cryptographic
> hashing relies on the avalanche effect of a small change in input
> value creating a drastic change in output value.


## Building

```bash
make
```

## Running

```bash
./bin/chash [image path] [-v] [-h]
```

Example:

```bash
$ ./bin/chash ./images/monkey.png
2ff36fde5a2749aa4536eb86ed465580
$
```

 - `-v` output version
 - `-h` display usage

## Installing

```bash
make install
```

## Uninstalling

```bash
make uninstall
```

## Cleaning

```bash
make clean
```

## Dependencies

 - libpng
 - libjpeg-turbo

#### Install on Debian

```
sudo apt install libpng-dev libjpeg-turbo-dev
```

#### Install on CentOS

```
sudo yum install libpng-devel libjpeg-turbo-devel
```

## License

    Copyright (C) 2019 Linnit <ryan@ryanl.co.uk>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

