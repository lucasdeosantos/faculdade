select tournament_name, award_name, family_name
from award_winners as aw, tournaments as t, awards as a, players as p
where aw.tournament_id = t.tournament_id and aw.award_id = a.award_id and aw.player_id = p.player_id;
