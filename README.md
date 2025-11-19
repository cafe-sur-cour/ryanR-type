## Commit Guidelines

This project follows the **Conventional Commit Message Guidelines**, which help maintain a clear and consistent Git history. The format is:

```
<type>(<scope>): <content>
```

### Common Types

- feat: A new feature
- fix: A bug fix
- docs: Documentation only changes
- style: Changes that do not affect the meaning of the code (white-space, formatting, etc.)
- refactor: Code changes that neither fix a bug nor add a feature
- test: Adding or correcting tests
- perf: A performance improvement
- upt: Updating an element that is not a refactor
- rm: Remove a line of code or a file from the repo
- memo: Add a new documentation element

## 🛠️ Git Commands Reference

### 🔄 Commit Management

**Modify commit message (before push):**
```bash
git commit --amend -m "New commit message"
```

**Modify commit message (after push):**
```bash
git commit --amend -m "New commit message"
git push --force
```

### 📂 File Management

**Unstage accidentally added file (not yet pushed):**
```bash
git restore --staged <file>
```

**Remove file from commit (after commit):**
```bash
git reset --soft HEAD~1
git restore --staged file-to-remove.txt
git commit -m "New commit message (without the file)"
```

## Documentation

### Docusaurus

The **Docusaurus** documentation regroups the entirety of the project documentation, protocole server gui and clinet information, but also how to contribute to the project and how to use the project itlsef.

To run the documentation travel to the `/documentation` folder than run the following :

``` bash
cd documentation/docusaurus/
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

You also need to make sur the the script is executable.
