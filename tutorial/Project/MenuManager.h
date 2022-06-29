class MenuManager
{
public:
    enum CameraSplitMode
    {
        no_split,
        split_x,
        split_y
    };

    static void OnCameraSplitChange(int splitMode)
    {
        switch (splitMode)
        {
        case CameraSplitMode::no_split:
            // implement
            break;
        case CameraSplitMode::split_x:
            // implement
            break;
        case CameraSplitMode::split_y:
            // implement
            break;
        default:
            // error
            break;
        }
    }

    static void OnAddCamera(std::string camera_name)
    {
        std::cout << "added camera" << camera_name << std::endl;
    }

    static void OnAddLayer(std::string layer_name,bool is_shown){

    }

    static void OnLayerChange(std::string layer_name,bool is_shown){

    }

    static void OnSelectMaterial(int material_inx){

    }
};