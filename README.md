# kubernetes-vars-deployer
A light way to deploy various environments on kubernetes using kubectl, vars and a config file

# Compile:
    * $gcc -o deployer source/main.c * 
# Usage:
    $deployer -c /path/to/config.conf | --config=/path/to/config.conf
    $deployer (only when vars.conf file is in exec dir)
    $deployer -m | --manual=yes (Manual mode)

# Why?:
    There is many reasons, the first one is to have untracked var files on
    massive kubernetes deploys, and need the posibility of making 
    "pull and deploy" strategy pointing to production or test vars such 
    subnets for load balancers, lb shapes, database host ips / fqdn.

# Recommendations:
    If it is ok that you can, do NOT use this to save passwords on config files.
    For general proposes, i dont want root creating files, so this code check 
    the uid and rejects root. You can bypass this by editing the code at main.
    maybe a future add as a parameter, but idts.
