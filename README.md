# libgit2

## Wrapper project for building our `libgit2` fork

### Building

* `sudo xcodebuild install`
* Retrieve root from `/tmp/xcode_libgit2.dst`
* `cd /path/to/root/Xcode.app && tar -zvcf Xcode_libgit2_DSTROOT_osx.tar.gz Xcode.app`

Use `sudo darwinup -p /path/to/target/xcode/dir install Xcode_libgit2_DSTROOT_osx.tar.gz` to install root for testing

Alternatively, use `swe roots`

* `swe atp build --override buildingInMonorail --build-only --no-radar --no-base-tag --notify <your_email>@apple.com -b CurrentSunriverE -p libgit2:<your_branch>`
* Wait for completion and email
* `swe roots install atp -dt -e <executions_url> [-x <path/to/Xcode.app>]`

### Submitting

Before submitting, it is recommended that you run `xbs buildit` with the latest submission changes. This will perform a local build that is the closest we can get to a B&I build, and might surface other issues that `xcodebuild` might not catch.

```
xbs buildit . -project libgit2-999 -archive -dsymsInDstroot -release $TRAIN
```

This will generate a root somewhere of XBS’ choosing, typically something like /tmp/libgit2-999.roots

You do not need to install this root. If an ATP root is performing correctly, you can proceed to submission.

To submit, use the following command where `X` is `${CURRENT_VERSION} + 1` and `TRAIN` is the Xcode train being submitted to:

```bash
xbs submitproject -version libgit2-${X} . ${TRAIN}
```

### Updating from Upstream

Setup the upstream remote:

```bash
git remote add upstream https://github.com/libgit2/libgit2.git
```

#### Non-breaking Changes

If the update is incremental, like updating from 1.8.1 to 1.8.2 for example, do the following:

```bash
git subtree pull --prefix=libgit2 upstream $RELEASE_TAG
```

#### Breaking Changes

If the update spans a minor version boundary, the safest move is to simply remove the subtree and re-add it. Newer `libgit2` minor versions will not merge cleanly into anything past the previous minor version's original release (i.e. 1.8.0 -> 1.9.0 is clean, 1.8.1 -> 1.9.0 is not clean).

```bash
git rm -r libgit2
git commit -m "[libgit2] Remove subtree preparing for minor version bump"
git subtree add --prefix=libgit2 upstream $RELEASE_TAG
```

#### Git Features File

libgit2 relies on cmake to render a header file called `git2_features.h` which toggles which implementation will be used for certain features (for example, GIT_SHA256_OPENSSL makes it use a openssl backing for sha256 hashes, and GIT_SHA256_COMMON_CRYPTO makes it use a commoncrypto backing for sha256).

In our clone, we disable this behavior by passing `LIBGIT2_NO_FEATURES_H` as one of the preprocessor macros, and enable each feature we want by manually passing in the feature flags as preprocessor macros.

It is possible that between different versions of libgit2 the necessary/desired feature flags will change, potentially even leading to compilation errors. It may be necesary to re-render it and see what libgit2 emits to see what we may want.

**Important**: the default cmake settings will disable the GIT_SSH feature flag, as the path for libssh2 must be furnished manually on setup. So don't rely on the output from a trivial cmake build to be an authoritative and correct output!

However, if end up needing to build libgit2 on desk, here are the trivial instructions:

1. install internal brew: https://github.pie.apple.com/homebrew/brew#installation-and-migration
2. install cmake: `$ brew install cmake`
3. build libgit using cmake:

```bash
mkdir build && cd build
cmake ../libgit2
cmake --build .
```

4. Open the newly rendered `git2_features.h` file found in `<libgit2_project>/build/src/util` and study what flags exist (both enabled and disabled), as some of those might be new and necessary.

#### Update OSS Plist

With all updates, the OSS plist needs to be updated to the correct tag and SHA for the new update:

```bash
$EDITOR libgit2.plist
git commit --amend libgit2.plist
```

You may also need to update the Xcode project if source files referenced by the project file are removed, or new ones need to be added.

