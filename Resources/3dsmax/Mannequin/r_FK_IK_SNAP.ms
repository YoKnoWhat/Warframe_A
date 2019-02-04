IK_FK_Switcher_Custom_attribute = attributes custom_Attributes
(
	rollout IKSwitchRollout "FK/IK Switch"
	(
		button ui_FK2IK "FK --> IK" width:160 height:30
		button ui_IK2FK "IK --> FK" width:160 height:30
		
		on ui_FK2IK pressed do 
		(
			try ($'upperarm_r_FK_ctrl'.transform = $'upperarm_r_IK'.transform) catch ()
			try ($'lowerarm_r_FK_ctrl'.transform = $'lowerarm_r_IK'.transform) catch ()
			try ($'hand_r_FK_ctrl'.transform = $'hand_r_IK'.transform) catch ()
		)
		
		on ui_IK2FK pressed do 
		(
			try ($'hand_r_IK_ctrl'.transform = $'hand_r_FK_ctrl'.transform) catch ()
		)
	)
)

CustAttributes.add $.modifiers[1] IK_FK_Switcher_Custom_attribute
