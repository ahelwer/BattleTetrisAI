from subprocess import call, Popen
from json import JSONDecoder, load

call(['wget', '-O', 'init.txt', 'http://codingcontest.pason.com/scheduler/create_unauthenticated_test_match?team_name=Team%20148&password=agrajabber'])
with open('init.txt', 'r') as jsf:
    raw = load(jsf)
server_ip = raw['server_ip']
match_token = raw['match_token']
Popen(['./a.out', server_ip, match_token])

