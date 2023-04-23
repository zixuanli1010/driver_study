cmd_/home/zixuan/driver_study/modules.order := {   echo /home/zixuan/driver_study/qat_driver.ko; :; } | awk '!x[$$0]++' - > /home/zixuan/driver_study/modules.order
