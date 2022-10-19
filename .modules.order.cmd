cmd_/home/zixuan/drv_study/modules.order := {   echo /home/zixuan/drv_study/demo_chr_dev.ko; :; } | awk '!x[$$0]++' - > /home/zixuan/drv_study/modules.order
