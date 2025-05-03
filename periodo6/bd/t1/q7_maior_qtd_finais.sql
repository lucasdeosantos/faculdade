select team_name, count(*) as qtd_finais
from qualified_teams as q, teams as t
where q.team_id = t.team_id and performance in('final', 'final round')
group by t.team_id
order by qtd_finais desc
limit 1;
