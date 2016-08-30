#!/usr/bin/env python3

import apt
import sys
import os
import subprocess

class ScribbleInstallProgress(apt.progress.base.InstallProgress):
    def fork(self):
        # add logging
        pid = os.fork()
        if pid == 0:
            logfile = '/dev/null'
            logfd = os.open(logfile, os.O_RDWR | os.O_APPEND | os.O_CREAT, 0o644)
            os.dup2(logfd, 1)
            os.dup2(logfd, 2)
        return pid

    def conffile(self, current, new):
        pass

    def error(self, pkg, errormsg):
        pass

    def processing(self, pkg, stage):
        pass

    def dpkg_status_change(self, pkg, status):
        pass

    def status_change(self, pkg, percent, status):
        print('[%s]: %d' % (pkg, percent))

    def start_update(self):
        pass

    def finish_update(self):
        pass


if not sys.argv[1:]:
    print('pass a package name to begin')
    sys.exit(0)

package_name = sys.argv[1]

print('package = %s' % package_name)

cache = apt.cache.Cache()

if package_name not in cache:
    print('could not find package')
    sys.exit(1)

if input('going to try to install package %s. ok? [y/N]' % package_name) != 'y': sys.exit(0)

package = cache[package_name]

print('package info:')
print('origins: %s' % package.versions[0].origins)

package.mark_install()

cache.commit(install_progress=ScribbleInstallProgress())

package.mark_delete()

cache.commit(install_progress=ScribbleInstallProgress())
