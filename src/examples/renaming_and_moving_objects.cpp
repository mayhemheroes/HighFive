#include <highfive/H5File.hpp>
#include <highfive/H5Group.hpp>
#include <highfive/H5DataSet.hpp>
#include <highfive/H5DataSpace.hpp>
#include <highfive/H5Attribute.hpp>

using namespace HighFive;

int main(void)
{
    /* We are going to create group in root directory then add
     * dataset to this group and attach attribute to the dataset.
     * Then we are trying to get path to the root, group dataset
     * and the name of the dataset.
     * Secondly we will move dataset with attached attribute to
     * some destination path.
     * To check if dataset object is still valid, we create a
     * second attribute */

    // Create a new file using the default property lists.
    File file("names.h5", File::ReadWrite | File::Create | File::Truncate);

    // Create a group
    Group group = file.createGroup("group");

    // Create a dummy dataset of one single integer
    DataSet dataset = group.createDataSet("data", DataSpace(1), AtomicType<int>());
    dataset.write(100);

    // Now let's add a attribute to this dataset
    std::string string_list("very important Dataset !");
    Attribute attribute = dataset.createAttribute<std::string>("attribute", DataSpace::From(string_list));
    attribute.write(string_list);

    // Get path and names
    std::cout << "root path: " << file.getPath() << std::endl;
    std::cout << "group path: " << group.getPath() << std::endl;
    std::cout << "dataset path: " << dataset.getPath() << std::endl;
    std::cout << "attribute name: " << attribute.getName() << std::endl;
    std::cout << std::endl;

    // Move dataset with its attribute to another destination path
    file.moveObject("/group/data", "/NewGroup/SubGroup/movedData");

    // As you can see to reach destination path new groups were created as well
    std::cout << "dataset new path: " << dataset.getPath() << std::endl;

    // We can still use moved dataset
    // let's create new attribute
    Attribute attributeNew = dataset.createAttribute<std::string>("attributeNew", DataSpace::From(string_list));
    attribute.write(string_list);
    std::cout << "attribute new name: " << attributeNew.getName() << std::endl;

    // move the folder with its content to other place
    file.moveObject("/NewGroup/SubGroup", "/FinalDestination");

    // Here is the important moment. The old 'dataset' variable tells us
    // that dataset directory wasn't changed
    std::cout << "dataset new path wasn't changed: " << dataset.getPath() << std::endl;

    // But actually it was moved we just need to update variable
    std::cout << "actually it was moved we just need to update it: " << file.getDataSet("/FinalDestination/movedData").getPath() << std::endl;
    std::cout << std::endl;

    file.flush();
}
