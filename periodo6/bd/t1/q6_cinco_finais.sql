select team_name, count(*)
from qualified_teams as q, teams as t
where q.team_id = t.team_id and performance in('final', 'final round')
group by t.team_id
having count(*) >= 5;
