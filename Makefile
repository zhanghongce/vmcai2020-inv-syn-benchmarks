vm:
	rm -rf configs/local.cfg
	cp configs/local-artifact.cfg configs/local.cfg
	bash BuildAll.sh

local:
	rm -rf configs/local.cfg
	cp configs/local-local.cfg configs/local.cfg
	bash BuildAll.sh
