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

        #['wave_num']カスタムプロパティを追加
        context.object["speed"] = 0.0

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