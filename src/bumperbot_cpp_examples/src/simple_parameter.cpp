#include <rclcpp/rclcpp.hpp>
#include <rcl_interfaces/msg/set_parameters_result.hpp>

#include <string>
#include <vector>
#include <memory>

using std::placeholders::_1;

class SimpleParameter : public rclcpp::Node
{
public:
    SimpleParameter(): Node("simple_parameter")
    {
        declare_parameter<int>("param_1",123);
        declare_parameter<std::string>("param_2","do not execute");

        param_callback_handle_ = add_on_set_parameters_callback(std::bind(&SimpleParameter::paramChangeCallback,this,_1));
    }
private:
    OnSetParametersCallbackHandle::SharedPtr param_callback_handle_;

    rcl_interfaces::msg::SetParametersResult paramChangeCallback(const std::vector<rclcpp::Parameter> &parameters)
    {
        rcl_interfaces::msg::SetParametersResult result;

        for(const auto& param:parameters)
        {
            if(param.get_name() == "param_1" && param.get_type() == rclcpp::ParameterType::PARAMETER_INTEGER)
            {
                RCLCPP_INFO_STREAM(get_logger(),"param_1 value changed, the value is : "<< param.as_int());
                result.successful = true;
            }
            if(param.get_name() == "param_2" && param.get_type() == rclcpp::ParameterType::PARAMETER_STRING)
            {
                RCLCPP_INFO_STREAM(get_logger(),"param_2 value changed, the value is : "<< param.as_string());
                result.successful = true;
            }
        }

        return result;
    }
};

int main(int argc, char* argv[]){
    rclcpp::init(argc,argv);
    auto node = std::make_shared<SimpleParameter>();
    rclcpp::spin(node);
    rclcpp::shutdown();

    return 0;
}