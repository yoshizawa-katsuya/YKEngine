#ブレンダーに登録するアドオン情報
bl_info = {
    "name": "レベルエディタ",
    "author": "Katsuya Yoshizawa",
    "version": (1, 0),
    "blender": (3, 3, 1),
    "location": "",
    "description": "レベルエディタ",
    "warning": "",
    "wiki_url": "",
    "tracker_url": "",
    "category": "Object"
}

#モジュールのインポート
import bpy
from .stretch_vertex import MYADDON_OT_stretch_vertex
from .create_ico_sphere import MYADDON_OT_create_ico_sphere
from .add_collider import MYADDON_OT_add_collider
from .add_filename import MYADDON_OT_add_filename
from .collider import OBJECT_PT_collider
from .draw_collider import DrawCollider
from .export_scene import MYADDON_OT_export_scene
from .filename import OBJECT_PT_file_name
from .my_menu import TOPBAR_MT_my_menu
from .disabled import MYADDON_OT_add_disabled
from .disabled import OBJECT_PT_disabled
from .spawn import SpawnNames
from .spawn import MYADDON_OT_spawn_import_symbol
from .spawn import MYADDON_OT_spawn_create_symbol
from .spawn import MYADDON_OT_spawn_create_player_symbol
from .spawn import MYADDON_OT_spawn_create_enemy_symbol
from .spawn import MYADDON_OT_spawn_create_enemy02_symbol
from .spawn import MYADDON_OT_spawn_create_tackle_enemy_symbol
from .params import MYADDON_OT_add_wait_time
from .params import OBJECT_PT_params
from .params import MYADDON_OT_add_wave_num
from .params import MYADDON_OT_add_speed

#Blenderに登録するクラスリスト
classes = (
    MYADDON_OT_stretch_vertex,
    MYADDON_OT_create_ico_sphere,
    MYADDON_OT_export_scene,
    TOPBAR_MT_my_menu,
    MYADDON_OT_add_filename,
    OBJECT_PT_file_name,
    MYADDON_OT_add_collider,
    OBJECT_PT_collider,
    MYADDON_OT_add_disabled,
    OBJECT_PT_disabled,
    MYADDON_OT_spawn_import_symbol,
    MYADDON_OT_spawn_create_symbol,
    MYADDON_OT_spawn_create_player_symbol,
    MYADDON_OT_spawn_create_enemy_symbol,
    MYADDON_OT_spawn_create_enemy02_symbol,
    MYADDON_OT_spawn_create_tackle_enemy_symbol,
    MYADDON_OT_add_wait_time,
    OBJECT_PT_params,
    MYADDON_OT_add_wave_num,
    MYADDON_OT_add_speed,
)

#メニュー項目描画
def draw_menu_manual(self, context):
    #self : 呼び出し元のクラスインスタンス。c++でいうthisポインタ
    #context : カーソルを合わせたときのポップアップのカスタマイズなどに使用

    #トップバーの「エディターメニュー」に項目（オペレータ）を追加
    self.layout.operator("wm.url_open_preset", text="Manual", icon="HELP")

#アドオン有効化時コールバック
def register():
    #Blenderにクラスを登録
    for cls in classes:
        bpy.utils.register_class(cls)

    #メニューに項目を追加
    bpy.types.TOPBAR_MT_editor_menus.append(TOPBAR_MT_my_menu.submenu)
    #3Dビューに描画関数を追加
    DrawCollider.handle = bpy.types.SpaceView3D.draw_handler_add(DrawCollider.draw_collider, (), "WINDOW", "POST_VIEW")
    print("レベルエディタが有効化されました。")

#アドオン無効化時コールバック
def unregister():
    #メニューから項目を削除
    bpy.types.TOPBAR_MT_editor_menus.remove(TOPBAR_MT_my_menu.submenu)
    #3Dビューから描画関数を削除
    bpy.types.SpaceView3D.draw_handler_remove(DrawCollider.handle, "WINDOW")

    #Blenderからクラスを削除
    for cls in classes:
        bpy.utils.unregister_class(cls)

    print("レベルエディタが無効化されました。")


#テスト実行用コード
if __name__ == "__main__":
    register()