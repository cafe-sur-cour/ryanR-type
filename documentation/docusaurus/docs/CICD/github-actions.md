---
sidebar_position: 1
---

## GitHub Workflow

To provide the best version possible, we guarantee continuous integration and deployment using a GitHub workflow. This workflow executes different jobs such as: executing cpp-linter, performing compilation, and running unit tests.

*This workflow also copies main content from our CSC repository to our EPITECH mirror repository*

### Jobs

Below is a description of each job:

**Check Repository**

This job checks if the workflow is started on the mirror repository or not. If it's the mirror repository, the next jobs won't be started:

```yml
check_repository:
    name: check repository to start jobs
    runs-on: ubuntu-latest
    outputs:
      target: ${{ steps.new_output.outputs.value }}
    steps:
      - name: CHECKOUT
        uses: actions/checkout@v4

      - name: CHECK TARGET REPOSITORY
        id: new_output
        run: |
          temp=$(echo $MIRROR_URL | cut -d ':' -f 2 | sed "s/\.git//")
          if [ $temp = ${{ github.repository }} ]; then
            echo "::notice title=The target repository is also the current one::$temp"
            echo "value=1" >> "$GITHUB_OUTPUT"
            exit 0
          fi
          echo "value=0" >> "$GITHUB_OUTPUT"
          exit 0
```

**Check Coding Style**

This job depends on the CHECK REPOSITORY job. It checks if a linter error occurs; if not, the pipeline will continue, otherwise it will be stopped:

```yml
check_coding_style:
    name: check any coding style errors
    needs: [check_repository]
    if: ${{ needs.check_repository.outputs.target == 0 }}
    runs-on: ubuntu-latest
    steps:
      - name: CHECKOUT
        uses: actions/checkout@v4

      - name: INSTALL CPP LINTER
        run: |
          apt update && apt install -y python3-pip
          pip install cpplint
          chmod +x ./scripts/cpp_style_checker.sh

      - name: CHECK CODING STYLE
        run: |
          ./scripts/cpp_style_checker.sh

      - name: VERIFY CPP CODING STYLE
        run: |
          if [ -s coding-style-cpp-reports.log ]; then
            echo "CPP coding style errors found. See coding-style-cpp-reports.log for details."
            cat coding-style-cpp-reports.log
            exit 1
          else
            echo "No CPP coding style errors found."
          fi
```

**Check Compilation**

This job depends on CHECK CODING STYLE. It compiles the program; if an error occurs, the pipeline will be stopped:

```yml
check_compilation:
    name: check project compilation
    needs: [check_coding_style]
    runs-on: ubuntu-latest
    steps:
      - name: CHECKOUT
        uses: actions/checkout@v4

      - name: INSTALL DEPENDENCIES
        run: |
          sudo apt install -y cmake g++ make git build-essential ninja-build libx11-dev libxi-dev libxrandr-dev libxcursor-dev libudev-dev libgl1-mesa-dev
          git clone https://github.com/Microsoft/vcpkg.git
          cd vcpkg
          ./bootstrap-vcpkg.sh
          echo "VCPKG_ROOT=$(pwd)" >> $GITHUB_ENV

      - name: RUN COMPILATION
        run: |
          chmod +x ./scripts/compile_project.sh
          ./scripts/compile_project.sh
```

**Run All Tests**

This job depends on CHECK COMPILATION. It runs all unit and functional tests; if a test fails, the pipeline will be stopped:

```yml
# This job should be updated when compilation script is ready
```

**Push To Mirror**

This job depends on RUN ALL TESTS and is skipped if the CI is not started on a push. It copies content from the */main branch of the CSC repository to the EPITECH repository (git@github.com:EpitechPGE3-2025/G-CPP-500-NAN-5-2-rtype-2.git):

```yml
push_to_mirror:
    name: push to target repository
    needs: [check_compilation]
    if: ${{ github.event_name == 'push' }}
    environment: RTYPE
    runs-on: ubuntu-latest
    steps:
      - name: CHECKOUT
        uses: actions/checkout@v4
        with:
          fetch-depth: 0
      - name: REPOSITORY MIRRORING
        uses: pixta-dev/repository-mirroring-action@v1
        with:
          target_repo_url: ${{ env.MIRROR_URL }}
          ssh_private_key: ${{ secrets.GIT_SSH_PRIVATE_KEY }}
```
