import bpy

#オペレータ カスタムプロパティ['wait_time']追加
class MYADDON_OT_add_wait_time(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_waittime"
    bl_label = "WaitTime 追加"
    bl_description = "['wait_time']カスタムプロパティを追加します"
    bl_options = {"REGISTER", "UNDO"}

    def execute(self, context):

        #['wait_time']カスタムプロパティを追加
        context.object["wait_time"] = 0.0

        return {"FINISHED"}
    

#オペレータ カスタムプロパティ['wave_num']追加
class MYADDON_OT_add_wave_num(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_wavenum"
    bl_label = "WaveNum 追加"
    bl_description = "['wave_num']カスタムプロパティを追加します"
    bl_options = {"REGISTER", "UNDO"}

    def execute(self, context):

        #['wave_num']カスタムプロパティを追加
        context.object["wave_num"] = 1

        return {"FINISHED"}  

#オペレータ カスタムプロパティ['speed']追加
class MYADDON_OT_add_speed(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_speed"
    bl_label = "Speed 追加"
    bl_description = "['speed']カスタムプロパティを追加します"
    bl_options = {"REGISTER", "UNDO"}

    def execute(self, context):

        #['speed']カスタムプロパティを追加
        context.object["speed"] = 0.0

        return {"FINISHED"}  

#オペレータ カスタムプロパティ['has_collider']追加
class MYADDON_OT_add_has_collider(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_has_collider"
    bl_label = "HasCollider 追加"
    bl_description = "['has_collider']カスタムプロパティを追加します"
    bl_options = {"REGISTER", "UNDO"}

    def execute(self, context):

        #['has_collider']カスタムプロパティを追加
        context.object["has_collider"] = True

        return {"FINISHED"} 

#オペレータ カスタムプロパティ['pose']追加
class MYADDON_OT_add_pose(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_pose"
    bl_label = "Pose 追加"
    bl_description = "['pose']カスタムプロパティを追加します"
    bl_options = {"REGISTER", "UNDO"}

    def execute(self, context):

        #['pose']カスタムプロパティを追加
        context.object["pose"] = ""

        return {"FINISHED"} 

#オペレータ カスタムプロパティ['feint_pose']追加
class MYADDON_OT_add_feint_pose(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_feint_pose"
    bl_label = "FeintPose 追加"
    bl_description = "['feint_pose']カスタムプロパティを追加します"
    bl_options = {"REGISTER", "UNDO"}

    def execute(self, context):

        #['feint_pose']カスタムプロパティを追加
        context.object["feint_pose"] = ""

        return {"FINISHED"} 


#パネル 待機時間
class OBJECT_PT_params(bpy.types.Panel):
    """オブジェクトのファイルネームパネル"""
    bl_name = "OBJECT_PT_params"
    bl_label = "Params"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    #サブメニューの描画
    def draw(self, context):

        #パネルに項目を追加
        if "wait_time" in context.object:
            #既にプロパティがあれば、プロパティを表示
            self.layout.prop(context.object, '["wait_time"]', text="wait_time")
        else:
            #プロパティがなければ、プロパティ追加ボタンを表示
            self.layout.operator(MYADDON_OT_add_wait_time.bl_idname)
        
        #パネルに項目を追加
        if "wave_num" in context.object:
            #既にプロパティがあれば、プロパティを表示
            self.layout.prop(context.object, '["wave_num"]', text="wave_num")
        else:
            #プロパティがなければ、プロパティ追加ボタンを表示
            self.layout.operator(MYADDON_OT_add_wave_num.bl_idname)

        #パネルに項目を追加
        if "speed" in context.object:
            #既にプロパティがあれば、プロパティを表示
            self.layout.prop(context.object, '["speed"]', text="speed")
        else:
            #プロパティがなければ、プロパティ追加ボタンを表示
            self.layout.operator(MYADDON_OT_add_speed.bl_idname)

        #パネルに項目を追加
        if "has_collider" in context.object:
            #既にプロパティがあれば、プロパティを表示
            self.layout.prop(context.object, '["has_collider"]', text="has_collider")
        else:
            #プロパティがなければ、プロパティ追加ボタンを表示
            self.layout.operator(MYADDON_OT_add_has_collider.bl_idname)

        #パネルに項目を追加
        if "pose" in context.object:
            #既にプロパティがあれば、プロパティを表示
            self.layout.prop(context.object, '["pose"]', text="pose")
        else:
            #プロパティがなければ、プロパティ追加ボタンを表示
            self.layout.operator(MYADDON_OT_add_pose.bl_idname)

        #パネルに項目を追加
        if "feint_pose" in context.object:
            #既にプロパティがあれば、プロパティを表示
            self.layout.prop(context.object, '["feint_pose"]', text="feint_pose")
        else:
            #プロパティがなければ、プロパティ追加ボタンを表示
            self.layout.operator(MYADDON_OT_add_feint_pose.bl_idname)