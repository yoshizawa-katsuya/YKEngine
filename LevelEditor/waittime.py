import bpy

#オペレータ カスタムプロパティ['wait_time']追加
class MYADDON_OT_add_wait_time(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_waittime"
    bl_label = "WaitTime 追加"
    bl_description = "['wait_time']カスタムプロパティを追加します"
    bl_options = {"REGISTER", "UNDO"}

    def execute(self, context):

        #['wait_time']カスタムプロパティを追加
        context.object["wait_time"] = 0

        return {"FINISHED"}
    
    #パネル ファイル名
class OBJECT_PT_wait_time(bpy.types.Panel):
    """オブジェクトのファイルネームパネル"""
    bl_name = "OBJECT_PT_wait_time"
    bl_label = "WaitTime"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    #サブメニューの描画
    def draw(self, context):

        #パネルに項目を追加
        if "wait_time" in context.object:
            #既にプロパティがあれば、プロパティを表示
            self.layout.prop(context.object, '["wait_time"]', text=self.bl_label)
        else:
            #プロパティがなければ、プロパティ追加ボタンを表示
            self.layout.operator(MYADDON_OT_add_wait_time.bl_idname)