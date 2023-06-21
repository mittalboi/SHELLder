## Report

+ Assuming that we do not need to handle symlinks for `ls`
+ Assuming that `pinfo` is only run for those processes for which the user has sufficent permissions to read statistics such as executable path.
+ Assuming hidden files are not expected as an output of `discover`
+ Assuming that no file named `.history` in the home folder is not in use by the user or any other process. The contents of this file will be modified.
+ The print statements informing the user about the status of background processes may overlap with input prompt or user input. This does not affect the functionality.
+ Multiple input and output redirection is not handled.
