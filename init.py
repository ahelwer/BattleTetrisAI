from subprocess import call
from json import JSONDecoder, load

call(['wget', '-O', 'init.txt', 'http://codingcontest.pason.com/scheduler/create_unauthenticated_test_match?team_name=Team%20148&password=agrajabber'])
with open('init.txt', 'r') as jsf:
    raw = load(jsf)
server_ip = raw['server_ip']
match_token = raw['match_token']
call(['./a.out', server_ip, match_token])
url = 'http://codingcontest.pason.com/tetris/index.html?mode=live&server_host=%(server_ip)s&bridge_host=ec2-174-129-115-197.compute-1.amazonaws.com&bridge_port=8000&game_id=%(match_token)s' %raw
print
print 'Watch match here:'
print
print url
print

