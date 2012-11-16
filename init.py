from subprocess import call
from json import JSONDecoder, load
from socket import gethostname

call(['wget', '-O', 'init.txt', 'http://codingcontest.pason.com/scheduler/create_unauthenticated_test_match?team_name=Team%20148&password=agrajabber'])
with open('init.txt', 'r') as jsf:
    raw = load(jsf)

if (gethostname() == 'domU-12-31-39-14-F8-BF'):
    print 'Using AWS-internal IP address.'
    server_ip = raw['internal_ip']
else:
    print 'Using normal IP address.'
    server_ip = raw['server_ip']

match_token = raw['match_token']
call(['./runTetris', server_ip, match_token])

