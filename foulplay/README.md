# foulplay

Decrypt FairPlay encrypted binaries on macOS when SIP-enabled.

By mapping an executable as r-x and then using `mremap_encrypted` on the encrypted
page(s) and then writing them back out to disk, you can fully decrypt FairPlay
binaries.

This was discovered independently when analyzing kernel sources, but it appears
that the technique was first introduced on iOS (but now works on macOS): https://github.com/JohnCoates/flexdecrypt
