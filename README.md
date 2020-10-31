# BrewOS-64
A 64-bit hobby operating system for the x86_64 architecture.

## Getting Started
1. Install [Docker](https://docs.docker.com/get-docker)

2. Set host IP address in .env file
`DISPLAY=<host ip address>:0.0`

3. Build container
`docker-compose build`

4. Start container
`docker-compose up -d build`

5. Access bash shell
`docker-compose exec build /bin/bash`

## Authors
* **Jamie** - *Initial work* - [coffeeunderrun](https://github.com/coffeeunderrun)

## License
This project is licensed under the GNU General Public License - see the [LICENSE](LICENSE) file for details