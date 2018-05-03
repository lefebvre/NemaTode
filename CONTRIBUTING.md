# Topic-based branch development
A topic branch is a short-lived branch that you create for a single focus of work.
Generally, short-lived is less than 6 months. The work can be bug fixes, feature development or design testing.
It is good practice to always use a branch when introducing changes.
This allows the master to remain passing and all subsequent branches to start clean.

## In practice
### Getting started
After you have cloned the project
```
git clone git@code-int.ornl.gov:nsm/fit fit
Cloning into 'fit'...
remote: Counting objects: 893, done.
remote: Compressing objects: 100% (384/384), done.
remote: Total 893 (delta 523), reused 852 (delta 484)
Receiving objects: 100% (893/893), 247.71 KiB | 95.00 KiB/s, done.
Resolving deltas: 100% (523/523), done.
Checking connectivity... done.
```
Checkout and create a local branch
```
git checkout -b contribution_guide
Switched to a new branch 'contribution_guide'
```
You now have a local branch `contribution_guide`
```
git branch -l
* contribution_guide
  master
```
This branch is not in the originating repository.
```
git branch -r
  origin/HEAD -> origin/master
  origin/master
```
### Making the first push
When you are ready to push your changes, you will get a fatal error about `no upstream branch`.
```
git push
fatal: The current branch contribution_guide has no upstream branch.
To push the current branch and set the remote as upstream, use

    git push --set-upstream origin contribution_guide
```
Follow the suggested command
```
git push --set-upstream origin contribution_guide
Counting objects: 6, done.
Delta compression using up to 8 threads.
Compressing objects: 100% (6/6), done.
Writing objects: 100% (6/6), 1.25 KiB | 0 bytes/s, done.
Total 6 (delta 3), reused 0 (delta 0)
remote:
remote: Create merge request for contribution_guide:
remote:   https://code-int.ornl.gov/nsm/fit/merge_requests/new?merge_request%5Bsource_branch%5D=contribution_guide
remote:
To git@code-int.ornl.gov:nsm/fit
 * [new branch]      contribution_guide -> contribution_guide
Branch contribution_guide set up to track remote branch contribution_guide from origin.
```
The remote server will return a quick link for creating a merge request if one hasn't already been created for this branch.


# Formatting
The use of the utility [clang-format](https://clang.llvm.org/docs/ClangFormat.html) makes for a quick processing of the desired format.
From the root directory running the following command will load default options for style formatting.
```
find . -type f -name "*.[hc][hc]" | grep fit | xargs clang-format -i -style=file
```
# Naming convention
## File naming
C++ files should end in `.cc` and header files in `.hh`.
Template implementations should end in `.i.hh`.
**File names should be all lowercase with no underscores.**

## Class naming
Classes should be **CamelCase**.
```c++
class CommandLine
{
};
```
Class members should be prefixed with **m** and be **lazyCamelCase**.
```c++
class CommandLine
{
private:
  int mArgc;
  char ** mArgv;
};
```
Class methods should be **lazyCamelCase**.
Class member getter methods should be the members name **lazyCamelCase**  `int argc() const`.
```c++
class Commandline
{
private:
  int mArgc;
public:
  int argc() const;
  int myMethod() const;
};
```
