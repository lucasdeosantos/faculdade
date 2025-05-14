select family_name, team_name
from squads as s, players as p, teams as t, tournaments as tr
where s.player_id = p.player_id and s.team_id = t.team_id and s.tournament_id = tr.tournament_id and s.shirt_number = 10 and tr.year = 2022;
