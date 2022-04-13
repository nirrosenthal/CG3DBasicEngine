import os
import glob
import shutil

BUILD_FOLDER = "cmake-build-debug"


def delete_folder_from_build(folder: str, build_folder: str):
    folder_path = os.path.join(build_folder, folder)
    if not os.path.isdir(folder_path):
        return
    shutil.rmtree(folder_path)


def copy_folder_to_build(folder: str, source_path: str, build_folder: str):
    source = os.path.join(source_path, folder)
    dest = os.path.join(build_folder, folder)
    shutil.copytree(source, dest)


def fix_shaders_postfix(build_folder: str):
    shaders_dir = os.path.join(build_folder, "shaders")
    files = glob.glob(os.path.join(shaders_dir, "**", "*.shader"), recursive=True)
    for f in files:
        os.rename(f, f.replace(".shader", ".vs"))


def main():
    source_folder = "tutorial"
    print("Copying shaders, textures and data...")
    delete_folder_from_build("shaders", BUILD_FOLDER)
    delete_folder_from_build("textures", BUILD_FOLDER)
    delete_folder_from_build("data", BUILD_FOLDER)

    copy_folder_to_build("shaders", source_folder, BUILD_FOLDER)
    copy_folder_to_build("textures", source_folder, BUILD_FOLDER)
    copy_folder_to_build("data", source_folder, BUILD_FOLDER)

    print("Done. Fixing '.shader' files to '.vs'...")
    fix_shaders_postfix(BUILD_FOLDER)
    print("Done")


if __name__ == "__main__":
    main()
