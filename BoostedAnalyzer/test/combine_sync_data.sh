cat syncElrunBtoG_.csv >  data.csv
for i in syncElrunH_.csv syncMuRunBtoG_.csv syncMuRunH_.csv syncElElRunBtoG_.csv syncElElRunH_.csv syncElMuRunBtoG_.csv syncElMuRunH_.csv syncMuMuRunBtoG_.csv syncMuMuRunH_.csv; do tail --lines=+2 $i >> data.csv; done