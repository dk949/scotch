# Repository Organisation

## Commit messages

General commit shape is:

    area: [area]: short sentence describing the change.

    Longer description. Reasons, implications, etc...

* Multiple areas can be provided if a commit affects multiple areas of the
  project
* Every line must not exceed 80 columns.
* Short message must start with a lowercase letter
* Long description must start with an upper case letter
* Do not use commit footer

### Areas

* List of areas and subareas for this project
  * build
  * ci
  * git
  * doxygen
  * readme
  * changelog
  * lexer
  * ast
  * parser
  * tools
  * test
  * wasm
  * general

## Changelog

Changelog style is based on
[keepachangelog.com](https://keepachangelog.com/en/1.0.0/)

### Headings

* Changes that are not yet part of any release are tracked in the first section.
  * Section heading: `## [Unreleased]`
* Changes that are a part of a release are tracked under the heading of that
  release.
  * Section heading: `## [<TAG_NUMBER>] - <DATE>`
* Releases are listed in chronological order latest to earliest.

### Subheadings

* Similar changes need to be grouped in headings
* All headings need to be in bold
* Types of headings are:
  * **Added**: new features.
  * **BREAKING**: changes that alter functionality so that user code has to be
    changed to accommodate it.
  * **Changed**: changes in existing functionality.
  * **Deprecated**: soon-to-be removed features.
  * **Removed**: removed features.
  * **Fixed**: bug fixes.
  * **Improved**: things that were not broken, but are now in a better state
    (e.g. refactor, better perf, etc.)
  * **Security**: in case of vulnerabilities.

### Tag/version numbers

* `<TAG_NUMBER>` = `v<VERSION_NUMBER>`
* Version number needs to adhere to [Semantic
  Versioning](https://semver.org/spec/v2.0.0.html)

### Dates

* Date format is ISO 8601 (yyyy-mm-dd)

### Links

* In order to improve readability of headings, short links are used. These links
  need to be defined at the bottom of the change log.
  * Order of links should be the same as the order of headings
* Link for `[Unreleased]` is:
  `[Unreleased]: https://github.com/<USERNAME>/<REPO_NAME>/compare/v<LATEST_TAG>...HEAD`
* If first tag has not been made yet, link for `[Unreleased]` is:
  `[Unreleased]: https://github.com/<USERNAME>/<REPO_NAME>`
* Link for `[<TAG>]` is:
  `[<TAG>]: https://github.com/<USERNAME>/<REPO_NAME>/compare/v<PREVIOUS_TAG>...v<TAG>`
* Link for the first tag is:
  `[<FIRST_TAG>]: https://github.com/<USERNAME>/<REPO_NAME>/releases/tag/v<FIRST_TAG>`
