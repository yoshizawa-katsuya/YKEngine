import bpy
import bpy.ops
import os

#データテーブル
class SpawnNames():
    #インデックス
    PROTOTYPE = 0   #プロトタイプのオブジェクト名
    INSTANCE = 1    #量産時のオブジェクト名
    FILENAME = 2    #リソースファイル名

    names = {}
    #names["キー"] = (プロトタイプのオブジェクト名、量産時のオブジェクト名、リソースファイル名)
    names["Enemy"] = ("PrototypeEnemySpawn", "EnemySpawn", "enemy/Enemy.obj")
    names["Player"] = ("PrototypePlayerSpawn", "PlayerSpawn", "player/Player.obj")
    names["TackleEnemy"] = ("PrototypeTackleEnemySpawn", "TackleEnemySpawn", "tackleEnemy/TackleEnemy.obj")


#オペレータ 出現ポイントのシンボルを読み込む
class MYADDON_OT_spawn_import_symbol(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_spawn_import_symbol"
    bl_label = "出現ポイントシンボルImport"
    bl_description = "出現ポイントのシンボルをImportします"

    def load_obj(self, spawn_type):
        print("出現ポイントのシンボルをImportします")

        #重複ロード防止
        spawn_object = bpy.data.objects.get(SpawnNames.names[spawn_type][SpawnNames.PROTOTYPE])
        if spawn_object is not None:
            return {'CANCELLED'}

        #スクリプトが配置されているディレクトリの名前を取得する
        addon_directory = os.path.dirname(__file__)
        #ディレクトリからのモデルファイルの相対パスを記述
        relative_path = SpawnNames.names[spawn_type][SpawnNames.FILENAME]
        #合成してモデルファイルのフルパスを得る
        full_path = os.path.join(addon_directory, relative_path)
        #オブジェクトをインポート
        bpy.ops.wm.obj_import('EXEC_DEFAULT',filepath=full_path, display_type='THUMBNAIL', forward_axis='NEGATIVE_Z', up_axis='Y')

        #回転を適用
        bpy.ops.object.transform_apply(location=False, rotation=True, scale=False, properties=False, isolate_users=False)

        #アクティブなオブジェクトを取得
        object = bpy.context.active_object
        #オブジェクト名を変更
        object.name = SpawnNames.names[spawn_type][SpawnNames.PROTOTYPE]

        #オブジェクトの種類を設定
        object["type"] = SpawnNames.names[spawn_type][SpawnNames.INSTANCE]

        #メモリ上に置いておくがシーンから外す
        bpy.context.collection.objects.unlink(object)

        return {'FINISHED'}
    
    def execute(self, context):
        #Enemyオブジェクト読み込み
        self.load_obj("Enemy")
        #Playerオブジェクト読み込み
        self.load_obj("Player")
        #TackleEnemyオブジェクト読み込み
        self.load_obj("TackleEnemy")

        return {'FINISHED'}
    
class MYADDON_OT_spawn_create_symbol(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_spawn_create_symbol"
    bl_label = "出現ポイントシンボルの作成"
    bl_description = "出現ポイントのシンボルを作成します"
    bl_options = {'REGISTER', 'UNDO'}
    
    #プロパティ
    spawn_type: bpy.props.StringProperty(name="Type", default="Player")# type: ignore

    def execute(self, context):
        #読み込み済みのコピー元オブジェクトを検索
        spawn_object = bpy.data.objects.get(SpawnNames.names[self.spawn_type][SpawnNames.PROTOTYPE])

        #まだ読み込んでいない場合
        if spawn_object is None:
            #読み込みオペレータを実行する
            bpy.ops.myaddon.myaddon_ot_spawn_import_symbol('EXEC_DEFAULT')
            #再建策
            spawn_object = bpy.data.objects.get(SpawnNames.names[self.spawn_type][SpawnNames.PROTOTYPE])
        
        print("出現ポイントのシンボルを作成します")

        #Blenderでの選択を解除する
        bpy.ops.object.select_all(action='DESELECT')

        #複製元の非表示オブジェクトを複製する
        object = spawn_object.copy()

        #複製したオブジェクトを現在のシーンにリンク(出現させる)
        bpy.context.collection.objects.link(object)

        #オブジェクト名を変更
        object.name = SpawnNames.names[self.spawn_type][SpawnNames.INSTANCE]

        return {'FINISHED'}
    
class MYADDON_OT_spawn_create_player_symbol(bpy.types.Operator):

    bl_idname = "myaddon.myaddon_ot_spawn_create_player_symbol"
    bl_label = "プレイヤー出現ポイントシンボルの作成"
    bl_description = "プレイヤー出現ポイントのシンボルを作成します"

    def execute(self, context):

        bpy.ops.myaddon.myaddon_ot_spawn_create_symbol('EXEC_DEFAULT', spawn_type = "Player")

        return {'FINISHED'}
    
class MYADDON_OT_spawn_create_enemy_symbol(bpy.types.Operator):

    bl_idname = "myaddon.myaddon_ot_spawn_create_enemy_symbol"
    bl_label = "敵出現ポイントシンボルの作成"
    bl_description = "敵出現ポイントのシンボルを作成します"

    def execute(self, context):

        bpy.ops.myaddon.myaddon_ot_spawn_create_symbol('EXEC_DEFAULT', spawn_type = "Enemy")

        return {'FINISHED'}
    
class MYADDON_OT_spawn_create_tackle_enemy_symbol(bpy.types.Operator):

    bl_idname = "myaddon.myaddon_ot_spawn_create_tackle_enemy_symbol"
    bl_label = "タックル型の敵出現ポイントシンボルの作成"
    bl_description = "タックル型の敵出現ポイントのシンボルを作成します"

    def execute(self, context):

        bpy.ops.myaddon.myaddon_ot_spawn_create_symbol('EXEC_DEFAULT', spawn_type = "TackleEnemy")

        return {'FINISHED'}