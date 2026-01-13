# Ryan R-Type

## Installing Ryan R-Type

### Via Releases

To install our project, you just need to go to our release page and follow the release's instructions to download it.

### Via cloning

If you want to install our project by cloning it, you will need to follow these steps after cloning, and inside the repository:

#### For unix systems
```bash
./scripts/install_dependencies.sh
./scripts/compile_project.sh
```

#### For windows
```cmd
.\scripts\install_dependencies.bat
.\scripts\compile_project.bat
```

### Running the Program

After compiling the project, you can run the server and client executables.

#### Running the Server
```bash
./r-type_server -p <port> -i <ip_address> -n <nb_clients>
```

Arguments:
- `-p <port>`: Specify the port number (required)
- `-i <ip_address>`: Specify the IP address to bind to (required)
- `-n <nb_clients>`: Specify the maximum number of clients (1-4, default: 1) (required)
- `-d`: Enable debug mode (optional)
- `-h`: Display help message (optional)

Example:
```bash
./r-type_server -p 4242 -i 127.0.0.1 -n 4
```

#### Running the Client
```bash
./r-type_client [options]
```

Arguments:
- `-p <port>`: Specify the port of the server (default: 4242) (optional)
- `-i <ip_address>`: Specify the IP address of the server (default: 127.0.0.1) (optional)
- `-n <name>`: Specify the name of the client (optional)
- `-d`: Enable debug mode (optional)
- `-h`: Display help message (optional)

Example:
```bash
./r-type_client -p 4242 -i 127.0.0.1 -n Player1
```

## Runing unit tests

```bash
# Basic coverage
./scripts/run_unit_tests.sh --coverage

# Coverage with HTML report
./scripts/run_unit_tests.sh --coverage --html

# Clean previous data and generate new coverage
./scripts/run_unit_tests.sh --clean-coverage --coverage --html
```

```bash
# Complete analysis with all reports
./scripts/coverage_analysis.sh
```

```bash
# Quick summary and open HTML report
./scripts/view_coverage.sh
```

## Contributing

For contribution guidelines, including prerequisites and commit conventions, please refer to [HOWTOCONTRIBUTE.md](HOWTOCONTRIBUTE.md).

## Documentation

### Docusaurus

The **Docusaurus** documentation regroups the entirety of the project documentation, protocole server gui and clinet information, but also how to contribute to the project and how to use the project itlsef.

Access the documentation at https://albanrss.github.io/doc-rtype/

To run the documentation travel to the `/documentation` folder than run the following :

``` bash
cd documentation/docusaurus/
npm install
npm start
```
Then you can find the documentation at http://localhost:3000/


#### Prerequisite :

To run the Docusaurus documentation you will need to have the docusaurus librairy installed on your device.

- [Docusaurus](https://docusaurus.io/docs/installation)


### Doxygen

The **Doxygen** module allows you to generate pdf documentation and schemas of the project architecure and logic.

To update the documentation run the following :

``` bash
./scripts/generateDoc.sh
```

#### Prerequisite :

To run the doxygen generations script you will need to have the Doxygen and pdf-Latex Library, you can find there install guide here

- [Doxygen](https://www.doxygen.nl/manual/install.html)
- [PDF Latex](https://www.latex-project.org/get/)

You also need to make sure that the script is executable.

``` bash
cd ./script
chmod +x generateDoc.sh
```
