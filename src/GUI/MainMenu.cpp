#include "MainMenu.h"

using namespace NCL;
using namespace CSC8503;

void MainMenu::Draw()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(360, 280), ImGuiCond_Always);

    if (!ImGui::Begin("Main Menu", NULL, window_flags))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
/*    
    if (ImGui::CollapsingHeader("Physics"))
    {
        if (ImGui::BeginTable("PhysicsTable", 3))
        {
            ImGui::TableNextColumn();

            ImGui::TableNextRow();
          
            ImGui::EndTable();
        }*/
        /*
        if (ImGui::Checkbox("Use Gravity", &(game->useGravity)))
            game->physics->UseGravity(game->useGravity);
        ImGui::SliderFloat("Push Force", &(game->forceMagnitude), 10.0f, 50000.0f);
        float gDamping = game->physics->GetGlobalDamping();
        if (ImGui::SliderFloat("Global Damping", &gDamping, 0.0f, 1.0f))
            game->physics->SetGlobalDamping(gDamping);
        float pScale = game->physics->GetPenaltyScale();
        if (ImGui::SliderFloat("Penalty Scale", &pScale, 0.0f, 5000.0f))
            game->physics->SetPenaltyScale(pScale);
        
    }
*/
    if (ImGui::CollapsingHeader("Render"))
    {
       /* static bool fullscreen = Window::GetWindow()->IsFullScreen();
        if (ImGui::Checkbox("Fullscreen", &fullscreen))
            Window::GetWindow()->SetFullScreen(fullscreen);
        if (ImGui::Button("ResetCamera"))
            game->InitCamera();*/
    }

   /* if (ImGui::CollapsingHeader("Help"))
    {
        ImGui::Text("Control Guide:");
        ImGui::BulletText("Use ESC to open pause menu.");
        ImGui::BulletText("Use A|S|D|W|Shift|Control to move camera.");
        ImGui::BulletText("Hold mouse right button to control camera rotation.");
        ImGui::BulletText("Use mouse left button to selected object.");
        ImGui::BulletText("Use space to push selected object.");
        ImGui::BulletText("Press L to lock/unlock selected object.");
        ImGui::BulletText("Use Up|Down|Left|Right to control the locked object.");
        ImGui::Separator();
    }

    if (game->selectionObject)
    {
        ImGui::Text("Selected Object:");
        std::string nameStr = "Name: " + game->selectionObject->GetName();
        ImGui::BulletText(nameStr.c_str());
        std::string posStr = "Position: " + game->selectionObject->GetTransform().GetPosition().ToString();
        ImGui::BulletText(posStr.c_str());
        std::string rotStr = "Rotation: " + game->selectionObject->GetTransform().GetOrientation().ToEuler().ToString();
        ImGui::BulletText(rotStr.c_str());
        if (game->selectionObject->GetPhysicsObject())
        {
            std::string invMassStr = "Inverse Mass: " + std::to_string(game->selectionObject->GetPhysicsObject()->GetInverseMass());
            ImGui::BulletText(invMassStr.c_str());
            std::string elasticityStr = "Elasticity: " + std::to_string(game->selectionObject->GetPhysicsObject()->GetElasticity());
            ImGui::BulletText(elasticityStr.c_str());
            std::string frictionStr = "Elasticity: " + std::to_string(game->selectionObject->GetPhysicsObject()->GetFriction());
            ImGui::BulletText(frictionStr.c_str());
            std::string lVelocityStr = "Linear Velocity: " + game->selectionObject->GetPhysicsObject()->GetLinearVelocity().ToString();
            ImGui::BulletText(lVelocityStr.c_str());
            std::string aVelocityStr = "Angular Velocity: " + game->selectionObject->GetPhysicsObject()->GetAngularVelocity().ToString();
            ImGui::BulletText(aVelocityStr.c_str());
        }*/
    //}

    ImGui::End();
}