import bpy

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
    
#パネル ウェーブ番号
class OBJECT_PT_wave_num(bpy.types.Panel):
    """オブジェクトのパネル"""
    bl_name = "OBJECT_PT_wave_num"
    bl_label = "WaveNum"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    #サブメニューの描画
    def draw(self, context):

        #パネルに項目を追加
        if "wave_num" in context.object:
            #既にプロパティがあれば、プロパティを表示
            self.layout.prop(context.object, '["wave_num"]', text=self.bl_label)
        else:
            #プロパティがなければ、プロパティ追加ボタンを表示
            self.layout.operator(MYADDON_OT_add_wave_num.bl_idname)