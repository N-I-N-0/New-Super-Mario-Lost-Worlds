.text

.extern checkpointActivated

.global SetCheckpointActivated

SetCheckpointActivated:
	lis r4, checkpointActivated@h
	ori r4, r4, checkpointActivated@l 
	stb r4, 0(r3)   #muesste das nicht so irgendwie: stb r4, 0(r3)   stb r4, r3 
	blr